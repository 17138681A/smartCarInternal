/*
 * five_way_switch.h
 *
 *  Created on: 6 Jan 2019
 *      Author: lch19
 */

#ifndef FIVE_WAY_SWITCH_H_
#define FIVE_WAY_SWITCH_H_

#include "driver/gpio.h"
#include "../config/config.h"
#include <functional>

#ifdef FIVE_WAY_SWITCH_USED

namespace DeviceDriver
{

/*!
 * @ingroup switch
 * @{
 */
/**
 * @brief Five way switch driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class FiveWaySwitch
{
  public:
	typedef std::function<void(bool state)> Switch_Listener;
	struct Config
	{
		uint8_t id;					/**< id of the five way switch */
		bool is_active_low = false; //!< true if the five way switch direction pin is in logic low when pressed to that direction \n
									//!< false if the five way switch direction pin is in logic high when pressed to that direction
		struct InterruptConfig
		{
			typedef Driver::GPIO::Config::Interrupt Interrupt;
			Switch_Listener listener[5] = {0};
			Interrupt interrupt_mode[5] = {Interrupt::kDisable, Interrupt::kDisable, Interrupt::kDisable, Interrupt::kDisable, Interrupt::kDisable};
			uint8_t interrupt_priority[5] = {15, 15, 15, 15, 15};
		};
		InterruptConfig interrupt_config;
	};
	enum Direction
	{
		kSelect,
		kUp,
		kDown,
		kLeft,
		kRight,
		kIdle
	};
	FiveWaySwitch(const Config &config);

	/**
	 * @brief Get the Direction of the five way switch is now pointing
	 * 
	 * @return FiveWaySwitch::Direction direction of the five way switch pointing
	 */
	Direction GetDirection() const;

  private:
	static Driver::GPIO::Config GetGPIOConfig(const Config &config, Direction dir);

	Driver::GPIO pin[5];
	bool is_active_low;
	Switch_Listener listener[5];
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* FIVE_WAY_SWITCH_H_ */
