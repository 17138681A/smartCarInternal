/*
 * five_way_switch.cpp
 *
 *  Created on: 6 Jan 2019
 *      Author: lch19
 */

#include "device_driver/five_way_switch.h"
#include <assert.h>

#ifdef FIVE_WAY_SWITCH_USED

namespace DeviceDriver
{

const System::Pinout::Name five_way_switch_pins[FIVE_WAY_SWITCH_USED * 5] = FIVE_WAY_SWITCH_PINS;

Driver::GPIO::Config FiveWaySwitch::GetGPIOConfig(const Config &config, Direction dir)
{
	assert(config.id < FIVE_WAY_SWITCH_USED);
	Driver::GPIO::Config gpio_config;
	gpio_config.gpio_dir = Driver::GPIO::Direction::kDigitalInput;
	gpio_config.pin = five_way_switch_pins[config.id * 5 + dir];
	return gpio_config;
}

FiveWaySwitch::FiveWaySwitch(const Config &config) : pin(
														 {GetGPIOConfig(config, kSelect), GetGPIOConfig(config, kUp), GetGPIOConfig(config, kDown), GetGPIOConfig(config, kLeft),
														  GetGPIOConfig(config, kRight)}),
													 is_active_low(config.is_active_low), listener(
																							  {config.interrupt_config.listener[0], config.interrupt_config.listener[1], config.interrupt_config.listener[2],
																							   config.interrupt_config.listener[3], config.interrupt_config.listener[4]})
{
	for (uint8_t i = 0; i < 5; i++)
	{
		if (listener[i])
		{
			pin[i].SetListener(config.interrupt_config.interrupt_mode[i], config.interrupt_config.listener[i],
							   config.interrupt_config.interrupt_priority[i]);
		}
	}
}

FiveWaySwitch::Direction FiveWaySwitch::GetDirection() const
{
	for (uint8_t i = 0; i < 5; i++)
	{
		if (pin[i].Get() ^ is_active_low)
		{
			return (Direction)i;
		}
	}
	return kIdle;
}

} // namespace DeviceDriver

#endif
