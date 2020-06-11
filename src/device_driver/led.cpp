/*
 * led.cpp
 *
 *  Created on: 18 Dec 2018
 *      Author: lch19
 */

#include "device_driver/led.h"

#ifdef LED_USED

namespace DeviceDriver
{

const System::Pinout::Name led_pins[LED_USED] = LED_PIN;

Driver::GPIO::Config Led::GetGPIOConfig(const Config &config)
{
	Driver::GPIO::Config gpio_config;
	gpio_config.pin = led_pins[config.id];
	gpio_config.gpio_dir = Driver::GPIO::Direction::kDigitalOutput;
	gpio_config.default_high = config.is_enable ^ config.is_active_low;
	System::Pinout::Config::PinConfig pin_config;
	pin_config.drive_strength = System::Pinout::Config::PinConfig::DriveStrength::kDSE1;
	pin_config.fast_slew_rate = true;
	pin_config.hysteresis_enable = false;
	pin_config.open_drain_enable = false;
	pin_config.speed = System::Pinout::Config::PinConfig::Speed::k200MHz;
	pin_config.pull_keep_config = System::Pinout::Config::PinConfig::PullKeepConfig::kKeep;
	gpio_config.pin_config = pin_config;
	return gpio_config;
}

Led::Led(const Config config) : led_gpio(GetGPIOConfig(config)), is_active_low(config.is_active_low)
{
}

} // namespace DeviceDriver

#endif
