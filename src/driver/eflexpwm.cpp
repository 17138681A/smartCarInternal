/*
 * eflexpwm.cpp
 *
 *  Created on: Aug 12, 2018
 *      Author: LeeChunHei
 */

#include "driver/eflexpwm.h"
#include "system/clock.h"
#include "system/system.h"
#include <assert.h>

namespace Driver
{

uint16_t eFlexPWM::pwm_modules_inited = 0;

bool eFlexPWM::GetModuleDetail(System::Pinout::Name pin, PWMModule &pwm_module, SubModule &sub_module, Channel &channel)
{
	System::Pinout::Config pin_config;
	pin_config.pin = pin;
	uint8_t mod, sub, chan;
	bool retval = System::Pinout::GeteFlexPWMPinConfig(pin_config, mod, sub, chan);
	pwm_module = (PWMModule)mod;
	sub_module = (SubModule)sub;
	channel = (Channel)chan;
	return retval;
}

bool eFlexPWM::CheckPin(System::Pinout::Name pin)
{
	PWMModule mod;
	SubModule sub;
	Channel channel;
	return GetModuleDetail(pin, mod, sub, channel);
}

eFlexPWM::eFlexPWM(const Config &config) : alignment(config.alignment)
{
	PWMModule pwm_module;
	assert(GetModuleDetail(config.pwm_pin, pwm_module, sub_module, channel));
	if ((pwm_modules_inited >> ((((uint8_t)pwm_module) << 2) + (uint8_t)sub_module)) & 0x1)
	{
		pwm_base = (PWM_Type *)(0x403DC000u + (uint8_t)(pwm_module)*0x4000);
		uint16_t reg = pwm_base->SM[(uint8_t)sub_module].CTRL;
		uint8_t prescale = (reg >> 4) & 0x7;
		period = (System::ipg_clock >> prescale) / config.frequency;
		InitChannel(channel, config.pwm_pin, config.polarity_inverted, config.duty_cycle, config.dead_time);
	}
	else
	{
		//Source clock for submodule 0 cannot be itself
		assert(config.clock_source != Config::ClockSource::kSubmodule0Clock || sub_module != SubModule::kSubModule0);
		//Reload source for submodule 0 cannot be master reload
		assert(!config.is_master_reload || sub_module != SubModule::kSubModule0);
		pwm_modules_inited |= 1 << ((((uint8_t)pwm_module) << 2) + (uint8_t)sub_module);
		System::CLOCK_EnableClock((System::clock_ip_name_t)(1040 + (uint8_t)(pwm_module)*2));
		pwm_base = (PWM_Type *)(0x403DC000u + (uint8_t)(pwm_module)*0x4000);
		pwm_base->FSTS |= PWM_FSTS_FFLAG_MASK;
		uint16_t reg = pwm_base->SM[(uint8_t)sub_module].CTRL2;
		reg &= ~(PWM_CTRL2_CLK_SEL_MASK | PWM_CTRL2_FORCE_SEL_MASK | PWM_CTRL2_INIT_SEL_MASK | PWM_CTRL2_INDEP_MASK | PWM_CTRL2_WAITEN_MASK | PWM_CTRL2_DBGEN_MASK | PWM_CTRL2_RELOAD_SEL_MASK);
		reg |= (PWM_CTRL2_CLK_SEL(config.clock_source) | PWM_CTRL2_FORCE_SEL(config.force_trigger) | PWM_CTRL2_INIT_SEL(config.initialization_control) | PWM_CTRL2_DBGEN(config.enable_debug_mode) | PWM_CTRL2_WAITEN(config.enable_wait) | PWM_CTRL2_RELOAD_SEL(config.is_master_reload));
		PWMModule temp_pwm_module;
		SubModule temp_sub_module;
		Channel temp_channel;
		if (config.comp_pwm_pin == System::Pinout::Name::kDisable)
		{
			reg |= PWM_CTRL2_INDEP_MASK;
		}
		else
		{
			assert(GetModuleDetail(config.comp_pwm_pin, temp_pwm_module, temp_sub_module, temp_channel));
			assert((temp_pwm_module == pwm_module) && (temp_sub_module == sub_module) && (temp_channel != channel));
			assert(channel != Channel::kPWMX);
			assert(temp_channel != Channel::kPWMX);
			if (channel == Channel::kPWMA)
			{
				pwm_base->MCTRL &= ~(1U << (PWM_MCTRL_IPOL_SHIFT + (uint8_t)sub_module)); //Complementary PWM A
			}
			else
			{
				pwm_base->MCTRL |= (1U << (PWM_MCTRL_IPOL_SHIFT + (uint8_t)sub_module)); //Complementary PWM B
			}
		}
		pwm_base->SM[(uint8_t)sub_module].CTRL2 = reg;
		reg = pwm_base->SM[(uint8_t)sub_module].CTRL;

		/* Setup the clock prescale, load mode and frequency */
		period = (System::ipg_clock >> 16) / config.frequency;
		uint8_t prescale = 0;
		for (; (period >> prescale) >= 1; ++prescale)
		{
		}
		period = (System::ipg_clock >> prescale) / config.frequency;
		uint16_t mod = period >> 1;
		if ((uint8_t)config.alignment % 2)
		{
			pwm_base->SM[(uint8_t)sub_module].INIT = 0;
			pwm_base->SM[(uint8_t)sub_module].VAL0 = mod;
			pwm_base->SM[(uint8_t)sub_module].VAL1 = period;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].INIT = -mod;
			pwm_base->SM[(uint8_t)sub_module].VAL0 = 0;
			pwm_base->SM[(uint8_t)sub_module].VAL1 = mod;
		}
		assert(prescale < 8);
		reg &= ~(PWM_CTRL_PRSC_MASK | PWM_CTRL_LDFQ_MASK | PWM_CTRL_LDMOD_MASK);
		reg |= (PWM_CTRL_PRSC(prescale) | PWM_CTRL_LDFQ(config.reload_frequence));

		/* Setup register reload logic */
		switch (config.reload_logic)
		{
		case Config::ReloadLogic::kReloadImmidate:
			reg |= PWM_CTRL_LDMOD_MASK;
			break;
		case Config::ReloadLogic::kReloadHalfCycle:
			reg |= PWM_CTRL_HALF_MASK;
			reg &= ~PWM_CTRL_FULL_MASK;
			break;
		case Config::ReloadLogic::kReloadFullCycle:
			reg &= ~PWM_CTRL_HALF_MASK;
			reg |= PWM_CTRL_FULL_MASK;
			break;
		case Config::ReloadLogic::kReloadHalfAndFullCycle:
			reg |= PWM_CTRL_HALF_MASK;
			reg |= PWM_CTRL_FULL_MASK;
			break;
		default:
			break;
		}
		pwm_base->SM[(uint8_t)sub_module].CTRL = reg;

		/* Setup the fault filter */
		if (pwm_base->FFILT & PWM_FFILT_FILT_PER_MASK)
		{
			/* When changing values for fault period from a non-zero value, first write a value of 0
			 * to clear the filter
			 */
			pwm_base->FFILT &= ~(PWM_FFILT_FILT_PER_MASK);
		}

		pwm_base->FFILT = (PWM_FFILT_FILT_CNT(0) | PWM_FFILT_FILT_PER(0));

		/* Issue a Force trigger event when configured to trigger locally */
		if (config.force_trigger == Config::ForceTrigger::kForce_Local)
		{
			pwm_base->SM[(uint8_t)sub_module].CTRL2 |= PWM_CTRL2_FORCE(1U);
		}

		pwm_base->MCTRL |= PWM_MCTRL_LDOK(1u << (uint8_t)sub_module);
		pwm_base->MCTRL |= PWM_MCTRL_RUN(1u << (uint8_t)sub_module);
		pwm_base->SM[(uint8_t)sub_module].DISMAP[0] = 0;
		pwm_base->SM[(uint8_t)sub_module].DISMAP[1] = 0;
		InitChannel(channel, config.pwm_pin, config.polarity_inverted, config.duty_cycle, config.dead_time);
		if (config.comp_pwm_pin != System::Pinout::Name::kDisable)
		{
			InitChannel(temp_channel, config.comp_pwm_pin, config.polarity_inverted, config.duty_cycle, config.dead_time);
		}
	}
}

void eFlexPWM::InitChannel(Channel channel, System::Pinout::Name pin_name, bool polarity_inverted, uint16_t duty_cycle, uint16_t dead_time)
{
	System::Pinout::Config pin_config;
	pin_config.pin = pin_name;
	uint8_t _module, _sub_module, _channel;
	System::Pinout::GeteFlexPWMPinConfig(pin_config, _module, _sub_module, _channel);
	System::Pinout::InitPin(pin_config);
	assert(duty_cycle <= 1000);
	uint16_t high_count = period * duty_cycle / 1000;
	switch (alignment)
	{
	case Config::Alignment::kSignedCenterAligned:
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = -(high_count >> 1);
			pwm_base->SM[(uint8_t)sub_module].VAL3 = high_count >> 1;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].VAL4 = -(high_count >> 1);
			pwm_base->SM[(uint8_t)sub_module].VAL5 = high_count >> 1;
		}
		break;
	case Config::Alignment::kCenterAligned:
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = (period - high_count) >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL3 = (period + high_count) >> 1;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].VAL4 = (period - high_count) >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL5 = (period + high_count) >> 1;
		}
		break;
	case Config::Alignment::kSignedEdgeAligned:
		if (channel == Channel::kPWMA)
		{
			uint16_t mod = period >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL2 = -mod;
			pwm_base->SM[(uint8_t)sub_module].VAL3 = -mod + high_count;
		}
		else
		{
			uint16_t mod = period >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL4 = -mod;
			pwm_base->SM[(uint8_t)sub_module].VAL5 = -mod + high_count;
		}
		break;
	case Config::Alignment::kEdgeAligned:
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = 0;
			pwm_base->SM[(uint8_t)sub_module].VAL3 = high_count;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].VAL4 = 0;
			pwm_base->SM[(uint8_t)sub_module].VAL5 = high_count;
		}
		break;
	default:
		break;
	}
	uint8_t polarity_shift;
	uint8_t output_enable_shift;
	if (channel == Channel::kPWMA)
	{
		polarity_shift = PWM_OCTRL_POLA_SHIFT;
		output_enable_shift = PWM_OUTEN_PWMA_EN_SHIFT;
		pwm_base->SM[(uint8_t)sub_module].DTCNT0 = PWM_DTCNT0_DTCNT0(dead_time);
	}
	else
	{
		polarity_shift = PWM_OCTRL_POLB_SHIFT;
		output_enable_shift = PWM_OUTEN_PWMB_EN_SHIFT;
		pwm_base->SM[(uint8_t)sub_module].DTCNT1 = PWM_DTCNT1_DTCNT1(dead_time);
	}

	/* Setup signal active level */
	if (polarity_inverted)
	{
		pwm_base->SM[(uint8_t)sub_module].OCTRL |= (1U << polarity_shift);
	}
	else
	{
		pwm_base->SM[(uint8_t)sub_module].OCTRL &= ~(1U << polarity_shift);
	}
	/* Enable PWM output */
	pwm_base->OUTEN |= (1U << (output_enable_shift + (uint8_t)sub_module));
	pwm_base->MCTRL |= PWM_MCTRL_LDOK(1u << (uint8_t)sub_module);
}

void eFlexPWM::SetDutyCycle(uint16_t duty_cycle)
{
	uint16_t high_count, pulse_cnt, mod; // = duty_cycle_in_percentage ? period * duty_cycle / 1000 : duty_cycle;
										 //	assert(high_count <= period);
	assert(duty_cycle <= 1000);
	pwm_base->MCTRL |= PWM_MCTRL_CLDOK(1u << (uint8_t)sub_module);
	switch (alignment)
	{
	case Config::Alignment::kSignedCenterAligned:
		pulse_cnt = (pwm_base->SM[(uint8_t)sub_module].VAL1) * 2;
		high_count = (pulse_cnt * duty_cycle) / 1000;
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = -(high_count >> 1);
			pwm_base->SM[(uint8_t)sub_module].VAL3 = high_count >> 1;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].VAL4 = -(high_count >> 1);
			pwm_base->SM[(uint8_t)sub_module].VAL5 = high_count >> 1;
		}
		break;
	case Config::Alignment::kCenterAligned:
		pulse_cnt = pwm_base->SM[(uint8_t)sub_module].VAL1;
		high_count = (pulse_cnt * duty_cycle) / 1000;
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = (pulse_cnt - high_count) >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL3 = (pulse_cnt + high_count) >> 1;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].VAL4 = (pulse_cnt - high_count) >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL5 = (pulse_cnt + high_count) >> 1;
		}
		break;
	case Config::Alignment::kSignedEdgeAligned:
		mod = pwm_base->SM[(uint8_t)sub_module].VAL1;
		pulse_cnt = mod >> 1;
		high_count = (pulse_cnt * duty_cycle) / 1000;
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = -mod;
			pwm_base->SM[(uint8_t)sub_module].VAL3 = -mod + high_count;
		}
		else
		{
			uint16_t mod = period >> 1;
			pwm_base->SM[(uint8_t)sub_module].VAL4 = -mod;
			pwm_base->SM[(uint8_t)sub_module].VAL5 = -mod + high_count;
		}
		break;
	case Config::Alignment::kEdgeAligned:
		pulse_cnt = pwm_base->SM[(uint8_t)sub_module].VAL1;
		high_count = (pulse_cnt * duty_cycle) / 1000;
		if (channel == Channel::kPWMA)
		{
			pwm_base->SM[(uint8_t)sub_module].VAL2 = 0;
			pwm_base->SM[(uint8_t)sub_module].VAL3 = high_count;
		}
		else
		{
			pwm_base->SM[(uint8_t)sub_module].VAL4 = 0;
			pwm_base->SM[(uint8_t)sub_module].VAL5 = high_count;
		}
		break;
	default:
		break;
	}
	pwm_base->MCTRL |= PWM_MCTRL_LDOK(1u << (uint8_t)sub_module);
}

} // namespace Driver
