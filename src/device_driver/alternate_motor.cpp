/*
 * alternate_motor.cpp
 *
 *  Created on: 31 Dec 2018
 *      Author: lch19
 */

#include "device_driver/alternate_motor.h"
#include "system/pinout/pinout.h"
#include "driver/eflexpwm.h"
#include "../config/config.h"
#include <assert.h>

#ifdef ALTERNATE_MOTOR_USED

namespace DeviceDriver
{

namespace AlternateMotorPin
{
System::Pinout::Name alternate_motor_pins[ALTERNATE_MOTOR_USED * 2] = ALTERNATE_MOTOR_PINS;
}

AlternateMotor::AlternateMotor(const Config &config)
{
	assert(config.id < ALTERNATE_MOTOR_USED);
	System::Pinout::Name pwm_a_pin = AlternateMotorPin::alternate_motor_pins[(config.id << 1)];
	System::Pinout::Name pwm_b_pin = AlternateMotorPin::alternate_motor_pins[(config.id << 1) + 1];
	if (Driver::eFlexPWM::CheckPin(pwm_a_pin))
	{
		Driver::eFlexPWM::Config eflexpwm_config;
		eflexpwm_config.pwm_pin = pwm_a_pin;
		eflexpwm_config.duty_cycle = 1000;
		eflexpwm_config.frequency = MOTOR_FREQ;
		eflexpwm_config.alignment = Driver::eFlexPWM::Config::Alignment::kEdgeAligned;
		eflexpwm_config.reload_logic = Driver::eFlexPWM::Config::ReloadLogic::kReloadImmidate;
		pwm_a = new Driver::eFlexPWM(eflexpwm_config);
	}
	else
	{
		assert(false);
	}
	if (Driver::eFlexPWM::CheckPin(pwm_b_pin))
	{
		Driver::eFlexPWM::Config eflexpwm_config;
		eflexpwm_config.pwm_pin = pwm_b_pin;
		eflexpwm_config.duty_cycle = 1000;
		eflexpwm_config.frequency = MOTOR_FREQ;
		eflexpwm_config.alignment = Driver::eFlexPWM::Config::Alignment::kEdgeAligned;
		eflexpwm_config.reload_logic = Driver::eFlexPWM::Config::ReloadLogic::kReloadImmidate;
		pwm_b = new Driver::eFlexPWM(eflexpwm_config);
	}
	else
	{
		assert(false);
	}
	speed = config.speed;
	SetSpeed(speed);
}

void AlternateMotor::SetSpeed(int32_t speed)
{
	this->speed = speed;
	if (speed < 0)
	{
		pwm_b->SetDutyCycle(1000);
		pwm_a->SetDutyCycle(1000 + speed);
	}
	else
	{
		pwm_a->SetDutyCycle(1000);
		pwm_b->SetDutyCycle(1000 - speed);
	}
}

void AlternateMotor::SetDirection(bool direction)
{
	if ((speed > 0) != direction)
	{
		speed *= -1;
	}
	SetSpeed(speed);
}

bool AlternateMotor::ToggleDirection()
{
	speed *= -1;
	SetSpeed(speed);
	return speed < 0;
}

} // namespace DeviceDriver

#endif
