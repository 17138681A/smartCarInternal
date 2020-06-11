/*
 * motor.h
 *
 *  Created on: 31 Dec 2018
 *      Author: lch19
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include "driver/timer.h"

namespace DeviceDriver
{

/*!
 * @addtogroup motor Motor
 * @ingroup device_driver
 * @{
 */
/**
 * @brief Protocol of motor driver
 */
class Motor
{
  public:
	struct Config
	{
		uint8_t id;
		int16_t speed = 0;
	};

	virtual ~Motor()
	{
	}

	/**
	 * @brief Set the motor speed
	 * 
	 * @param speed: speed that want the motor to output. \n
	 *               range -1000 to 1000, means power output -100.0% to 100.0%
	 */
	virtual void SetSpeed(int32_t speed) = 0;

	/**
	 * @brief Set the motor turning direction
	 * 
	 * @param direction: direction that want the motor to turn \n
	 *                   true means final speed is positive \n
	 *                   false means final speed is negative
	 */
	virtual void SetDirection(bool direction) = 0;

	/**
	 * @brief Toggle the motor turning direction
	 * 
	 * @return true:  final speed is positive
	 * @return false: final speed is negative
	 */
	virtual bool ToggleDirection() = 0;

	/**
	 * @brief Get the motor speed
	 * 
	 * @return int16_t motor speed
	 */
	inline int16_t GetSpeed() const
	{
		return speed;
	}

	/**
	 * @brief Get the motor turning direction
	 * 
	 * @return true:  speed is positive
	 * @return false: speed is negative
	 */
	inline bool GetDirection() const
	{
		return speed > 0;
	}

  protected:
	int16_t speed;
};
/** @}*/

} // namespace DeviceDriver

#endif /* MOTOR_H_ */
