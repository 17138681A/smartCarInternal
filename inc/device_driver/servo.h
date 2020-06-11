/*
 * servo.h
 *
 *  Created on: 28 Jan 2019
 *      Author: lch19
 */

#ifndef DEVICE_DRIVER_SERVO_H_
#define DEVICE_DRIVER_SERVO_H_

#include "driver/timer.h"
#include "../config/config.h"

#ifdef SERVO_USED

namespace DeviceDriver
{

/*!
 * @addtogroup servo Servo
 * @ingroup device_driver
 * @{
 */
/**
 * @brief Servo driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class Servo
{
  public:
	struct Config
	{
		uint8_t id;
		uint16_t degree = 900;
		uint32_t frequency;		/*!<In Hz */
		uint32_t min_pos_width; /*!<In μs */
		uint32_t max_pos_width; /*!<In μs */
	};
	explicit Servo(const Config &config);

	/**
	 * @brief Set the servo degree
	 * 
	 * @param degree: degree that want the servo to turn \n
	 *                range 0 to 1800, means servo degree 0.0° to 180.0°
	 */
	inline void SetDegree(uint16_t degree)
	{
		this->degree = (degree > 1800 ? 1800 : degree);
		pwm->SetDutyCycle(this->degree * convert_ratio + min_pos);
	}

	/**
	 * @brief Get the servo degree that set before
	 * 
	 * @return uint16_t degree of the servo, range 0 to 1800, means servo degree 0.0° to 180.0°
	 */
	inline uint16_t GetDegree() const
	{
		return degree;
	}

  private:
	Driver::Timer *pwm;
	const float min_pos;
	const float convert_ratio;
	uint16_t degree;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* DEVICE_DRIVER_SERVO_H_ */
