/*
 * switch.h
 *
 *  Created on: 6 Jan 2019
 *      Author: lch19
 */

#ifndef DEVICE_DRIVER_SWITCH_H_
#define DEVICE_DRIVER_SWITCH_H_

#include "driver/gpio.h"
#include "../config/config.h"
#include <functional>

#ifdef SWITCH_USED

namespace DeviceDriver
{

/*!
 * @addtogroup switch Switch
 * @ingroup device_driver
 * @{
 */
/**
 * @brief Switch driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class Switch
{
  public:
	typedef std::function<void(bool state)> Switch_Listener;
	struct Config
	{
		uint8_t id;					/*!<id of the switch */
		bool is_active_low = false; /*!<true if the switch is in logic low when pressed \n*/
									/*!<false if the switch is in logic high when pressed */
		struct InterruptConfig
		{
			typedef Driver::GPIO::Config::Interrupt Interrupt;
			Switch_Listener listener = 0;
			Interrupt interrupt_mode = Interrupt::kDisable;
			uint8_t interrupt_priority = 15;
		};
		InterruptConfig interrupt_config;
	};
	Switch(const Config &config);

	/**
	 * @brief Get the switch state
	 * 
	 * @return true:  switch is pressed
	 * @return false: switch is not pressed
	 */
	inline bool Get()
	{
		return gpio.Get() ^ is_active_low;
	}

  private:
	static Driver::GPIO::Config GetGPIOConfig(uint8_t id);

	Driver::GPIO gpio;
	const bool is_active_low;
	Switch_Listener listener;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* DEVICE_DRIVER_SWITCH_H_ */
