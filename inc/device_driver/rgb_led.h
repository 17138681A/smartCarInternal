/*
 * rgb_led.h
 *
 *  Created on: 2 Jan 2019
 *      Author: lch19
 */

#ifndef RGB_LED_H_
#define RGB_LED_H_

#include "driver/gpio.h"
#include "../config/config.h"

#ifdef RGB_USED

namespace DeviceDriver
{

/*!
 * @ingroup led
 * @{
 */
/**
 * @brief RGB led driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class RGBLed
{
  public:
	struct Config
	{
		uint8_t id;
		bool is_enable = true;		/*!<State of led when construsted, true->led on, false->led off */
		bool is_active_low = true;	/*!<true when led need to light up with logic low ouput(Led anode connect to GND), false when led need to light up with logic high output(Led cathode connect to GND) */
	};

	enum Color
	{
		kWhite,
		kRed,
		kGreen,
		kBlue,
		kYellow,
		kMagenta,
		kCyan
	};

	explicit RGBLed(const Config &config);

	/**
	 * @brief Set the state of the rgb led
	 * 
	 * @param active: led on (white light) when true, led off when false 
	 */
	inline void Set(const bool active)
	{
		r.Set(is_active_low ^ active);
		g.Set(is_active_low ^ active);
		b.Set(is_active_low ^ active);
	}

	/**
	 * @brief Set the color of the rgb led
	 * 
	 * @param color: the color that will light up
	 */
	void SetColor(const Color &color);

  private:
	static Driver::GPIO::Config GetGPIOConfig(const Config &config, System::Pinout::Name pin);

	Driver::GPIO r, g, b;
	const bool is_active_low;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* RGB_LED_H_ */
