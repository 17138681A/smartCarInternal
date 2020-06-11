/*
 * led.h
 *
 *  Created on: 18 Dec 2018
 *      Author: lch19
 */

#ifndef DEVICE_DRIVER_LED_H_
#define DEVICE_DRIVER_LED_H_

#include "driver/gpio.h"
#include "../config/config.h"

#ifdef LED_USED

namespace DeviceDriver
{

/*!
 * @addtogroup led LED
 * @ingroup device_driver
 * @{
 */
/**
 * @brief LED driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class Led
{
  public:
	struct Config
	{
		uint8_t id;				   //id of the led
		bool is_enable = true;	 //State of led when construsted, true->led on, false->led off.
		bool is_active_low = true; //True when led need to light up with logic low ouput(Led anode connect to GND), False when led need to light up with logic high output(Led cathode connect to GND).
	};
	Led(const Config config);

	/**
	 * @brief Set the state of the led
	 * 
	 * @param active: led on when true, led off when false
	 */
	void Set(bool active)
	{
		led_gpio.Set(is_active_low ^ active);
	}
	
	/**
	 * @brief Toggle the state of the led
	 * 
	 * @return true:  led is on
	 * @return false: led is off
	 */
	bool Toggle()
	{
		return led_gpio.Toggle() ^ is_active_low;
	}

  private:
	static Driver::GPIO::Config GetGPIOConfig(const Config &config);

	Driver::GPIO led_gpio;
	const bool is_active_low;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* DEVICE_DRIVER_LED_H_ */
