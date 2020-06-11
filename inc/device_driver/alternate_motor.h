/*
 * alternate_motor.h
 *
 *  Created on: 31 Dec 2018
 *      Author: lch19
 */

#ifndef ALTERNATE_MOTOR_H_
#define ALTERNATE_MOTOR_H_

#include "device_driver/motor.h"
#include "../config/config.h"

#ifdef ALTERNATE_MOTOR_USED

namespace DeviceDriver
{

/*!
 * @ingroup motor
 * @{
 */
/**
 * @brief Alternate motor driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * AlternateMotor::Config config;
 * config.id = 0;
 * config.speed = 0;
 * AlternateMotor motor(config);
 * motor.SetSpeed(1000);				//motor is now running at 100.0%
 * motor.SetSpeed(-500);				//motor is now running at -50.0%
 * bool dir = motor.ToggleDirection();	//motor turn the opposite direction now
 * ~~~~~~~~~~~~~~~
 */
class AlternateMotor : public Motor
{
  public:
	AlternateMotor(const Config &config);

	/**
	 * @brief Set the motor speed
	 * 
	 * @param speed speed that want the motor to output \n
	 * 		  range  -1000 to 1000, means power output -100.0% to 100.0%
	 */
	void SetSpeed(int32_t speed) override;

	/**
	 * @brief Set the motor turning direction
	 * 
	 * @param direction direction that want the motor to turn \n
	 *                  true means final speed is positive, PWMA will have output \n
	 *                  false means final speed is negative, PWMB will have output
	 */
	void SetDirection(bool direction) override;

	/**
	 * @brief Toggle the motor turning direction
	 * 
	 * @return true final speed is positive, PWMA will have output
	 * @return false final speed is negative, PWMB will have output
	 */
	bool ToggleDirection() override;

  private:
	Driver::Timer *pwm_a;
	Driver::Timer *pwm_b;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* ALTERNATE_MOTOR_H_ */
