/*
 * adc.cpp
 *
 *  Created on: Aug 23, 2018
 *      Author: LeeChunHei
 */

#include "driver/adc.h"
#include "system/clock.h"

namespace Driver
{

ADC *adc_ptr[2] = {};

ADC::ADC(Config &config)
{
	System::CLOCK_EnableClock((bool)(config.adc_module) ? System::kCLOCK_Adc2 : System::kCLOCK_Adc1);
	adc_base = (bool)(config.adc_module) ? ADC2 : ADC1;
	uint32_t reg = adc_base->CFG & (ADC_CFG_AVGS_MASK | ADC_CFG_ADTRG_MASK);
	reg |= ADC_CFG_REFSEL(0u) | ADC_CFG_ADSTS((uint8_t)(config.sample_period_mode)) | ADC_CFG_ADICLK((uint8_t)(config.clock_source)) | ADC_CFG_ADIV((uint8_t)(config.clock_divider)) | ADC_CFG_MODE((uint8_t)(config.resolution));
	reg |= (0 | config.enable_overwrite << 16 | config.enable_high_speed << 10 | config.enable_low_power << 7 | config.enable_long_sample << 4);
	adc_base->CFG = reg;
	reg = adc_base->GC & ~(ADC_GC_ADCO_MASK | ADC_GC_ADACKEN_MASK);
	reg |= (0 | config.enable_continuous_conversion << 6 | config.enable_asynchronous_clock_output);
	adc_base->GC = reg;
	adc_ptr[(uint8_t)(config.adc_module)] = this;
	listener = new ADC_Listener[8];
}

void ADC::ConfigHWTrigger(bool enable)
{
	if (enable)
	{
		adc_base->CFG |= ADC_CFG_ADTRG_MASK;
	}
	else
	{
		adc_base->CFG &= ~ADC_CFG_ADTRG_MASK;
	}
}

bool ADC::AutoCalibration()
{
	bool result = true;
	bool hw_trigger = (ADC_CFG_ADTRG_MASK & adc_base->CFG);

	/* The calibration would be failed when in hardwar mode.
	 * Remember the hardware trigger state here and restore it later if the hardware trigger is enabled.*/
	if (hw_trigger)
	{
		ConfigHWTrigger(false);
	}

	/* Clear the CALF and launch the calibration. */
	adc_base->GS = ADC_GS_CALF_MASK; /* Clear the CALF. */
	adc_base->GC |= ADC_GC_CAL_MASK; /* Launch the calibration. */

	/* Check the status of CALF bit in ADC_GS and the CAL bit in ADC_GC. */
	while (adc_base->GC & ADC_GC_CAL_MASK)
	{
		/* Check the CALF when the calibration is active. */
		if (adc_base->GS & ADC_GS_CALF_MASK)
		{
			result = false;
			break;
		}
	}

	/* When CAL bit becomes '0' then check the CALF status and COCO[0] bit status. */
	if (!(adc_base->HS & 1u)) /* Check the COCO[0] bit status. */
	{
		result = false;
	}
	if (adc_base->GS & ADC_GS_CALF_MASK) /* Check the CALF status. */
	{
		result = false;
	}

	/* Clear conversion done flag. */
	volatile uint32_t clear = adc_base->R[0];

	clear++; //use to remove variable not use warning

	/* Restore original trigger mode. */
	if (hw_trigger)
	{
		ConfigHWTrigger(true);
	}

	return result;
}

bool ADC::ConfigChannel(Channel channel, Input input, ADC_Listener listener, uint8_t interrupt_priority)
{
	if (listener)
	{
		this->listener[(uint8_t)channel] = listener;
		NVIC_EnableIRQ(adc_base == ADC1 ? ADC1_IRQn : ADC2_IRQn);
		NVIC_SetPriority(adc_base == ADC1 ? ADC1_IRQn : ADC2_IRQn, interrupt_priority);
	}
	adc_base->HC[(uint8_t)channel] = (0 | (uint8_t)input | (bool)listener << 7);
	if (!(opened_input_pin[(uint8_t)input]))
	{
		opened_input_pin[(uint8_t)input] = true;
		return OpenADCInputPin(GetInputPinName(input));
	}
	else
	{
		return true;
	}
}

bool ADC::OpenADCInputPin(System::Pinout::Name pin_name)
{
	System::Pinout::Config config;
	config.pin = pin_name;
	if (System::Pinout::GetADCPinConfig(config))
	{
		System::Pinout::InitPin(config);
		return true;
	}
	else
	{
		return false;
	}
}

System::Pinout::Name ADC::GetInputPinName(Input input)
{
	if (adc_base == ADC1)
	{
		if (input == Input::kInput0)
		{
			return System::Pinout::Name::kGPIO_AD_B1_11;
		}
		else
		{
			return (System::Pinout::Name)((uint8_t)input + 53);
		}
	}
	else
	{
		if (input <= Input::kInput4)
		{
			return (System::Pinout::Name)((uint8_t)input + 69);
		}
		else
		{
			return (System::Pinout::Name)((uint8_t)input + 53);
		}
	}
}

extern "C"
{

	void ADC1_IRQHandler()
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			if (((ADC1->HS) & (1U << i)) >> i)
			{
				adc_ptr[0]->GetListener()[i](adc_ptr[0]);
				if (!ADC1->HS)
					break;
			}
		}
	}

	void ADC2_IRQHandler()
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			if (((ADC2->HS) & (1U << i)) >> i)
			{
				adc_ptr[1]->GetListener()[i](adc_ptr[1]);
				if (!ADC2->HS)
					break;
			}
		}
	}
}

} // namespace Driver
