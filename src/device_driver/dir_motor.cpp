/*
 * dir_motor.cpp
 *
 *  Created on: 31 Dec 2018
 *      Author: LeeChunHei
 */

#include "device_driver/dir_motor.h"
#include "system/pinout/pinout.h"
#include "driver/eflexpwm.h"
#include <assert.h>

#ifdef DIR_MOTOR_USED

namespace DeviceDriver
{

namespace DirMotorPin
{
System::Pinout::Name dir_motor_pins[DIR_MOTOR_USED * 2] = DIR_MOTOR_PINS;
}

DirMotor::DirMotor(const Config &config)
{
	assert(config.id < DIR_MOTOR_USED);
	System::Pinout::Name dir_pin = DirMotorPin::dir_motor_pins[(config.id << 1)];
	System::Pinout::Name pwm_pin = DirMotorPin::dir_motor_pins[(config.id << 1) + 1];
	Driver::GPIO::Config gpio_config;
	gpio_config.pin = dir_pin;
	gpio_config.gpio_dir = Driver::GPIO::Direction::kDigitalOutput;
	dir = new Driver::GPIO(gpio_config);
	if (Driver::eFlexPWM::CheckPin(pwm_pin))
	{
		Driver::eFlexPWM::Config eflexpwm_config;
		eflexpwm_config.pwm_pin = pwm_pin;
		eflexpwm_config.duty_cycle = 0;
		eflexpwm_config.frequency = MOTOR_FREQ;
		eflexpwm_config.alignment = Driver::eFlexPWM::Config::Alignment::kEdgeAligned;
		pwm = new Driver::eFlexPWM(eflexpwm_config);
	}
	else
	{
		assert(false);
	}
	speed = config.speed;
	SetSpeed(speed);
}

void DirMotor::SetSpeed(int32_t speed)
{
	this->speed = speed;
	if (speed > 0)
	{
		dir->Set(true);
		pwm->SetDutyCycle(speed);
	}
	else
	{
		dir->Set(false);
		pwm->SetDutyCycle(-speed);
	}
}

void DirMotor::SetDirection(bool direction)
{
	if ((speed > 0) != direction)
	{
		speed *= -1;
	}
	dir->Set(speed > 0);
}

bool DirMotor::ToggleDirection()
{
	speed *= -1;
	return dir->Toggle();
}

} // namespace DeviceDriver

#endif
