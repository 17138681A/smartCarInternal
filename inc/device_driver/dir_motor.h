/*
 * dir_motor.h
 *
 *  Created on: 31 Dec 2018
 *      Author: lch19
 */

#ifndef DEVICE_DRIVER_DIR_MOTOR_H_
#define DEVICE_DRIVER_DIR_MOTOR_H_

#include "device_driver/motor.h"
#include "driver/gpio.h"
#include "../config/config.h"

#ifdef DIR_MOTOR_USED

namespace DeviceDriver
{

/*!
 * @ingroup motor
 * @{
 */
/**
 * @brief Direction motor driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class DirMotor : public Motor
{
  public:
	DirMotor(const Config &config);

	/**
	 * @brief Set the motor speed
	 * @param speed: speed that want the motor to output \n
	 *               range -1000 to 1000, means power output -100.0% to 100.0%
	 */
	void SetSpeed(int32_t speed) override;

	/**
	 * @brief Set the motor turning direction
	 * 
	 * @param direction: direction that want the motor to turn \n
	 *                   true means final speed is positive \n
	 *                   false means final speed is negative
	 */
	void SetDirection(bool direction) override;

	/**
	 * @brief Toggle the motor turning direction
	 * 
	 * @return true:  final speed is positive
	 * @return false: final speed is negative
	 */
	bool ToggleDirection() override;

  private:
	Driver::Timer *pwm;
	Driver::GPIO *dir;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* DEVICE_DRIVER_DIR_MOTOR_H_ */
