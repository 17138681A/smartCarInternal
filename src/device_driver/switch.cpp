/*
 * switch.cpp
 *
 *  Created on: 6 Jan 2019
 *      Author: lch19
 */

#include "device_driver/switch.h"
#include <assert.h>

#ifdef SWITCH_USED

namespace DeviceDriver
{

const System::Pinout::Name switch_pin[SWITCH_USED] = SWITCH_PIN;

Driver::GPIO::Config GetGPIOConfig(uint8_t id)
{
	assert(id < SWITCH_USED);
	Driver::GPIO::Config config;
	config.gpio_dir = Driver::GPIO::Direction::kDigitalInput;
	config.pin = switch_pin[id];
	return config;
}

Switch::Switch(const Config &config) : gpio(GetGPIOConfig(config.id)), is_active_low(config.is_active_low), listener(config.interrupt_config.listener)
{
	if (listener)
	{
		gpio.SetListener(config.interrupt_config.interrupt_mode, config.interrupt_config.listener, config.interrupt_config.interrupt_priority, true);
	}
}

} // namespace DeviceDriver

#endif
