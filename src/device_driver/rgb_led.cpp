/*
 * rgb_led.cpp
 *
 *  Created on: 2 Jan 2019
 *      Author: lch19
 */

#include "device_driver/rgb_led.h"

#ifdef RGB_USED

namespace DeviceDriver
{

System::Pinout::Name rgb_pins[RGB_USED * 3] = RGB_LED_PINS;

Driver::GPIO::Config RGBLed::GetGPIOConfig(const Config &config, System::Pinout::Name pin)
{
	Driver::GPIO::Config gpio_config;
	gpio_config.pin = pin;
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

RGBLed::RGBLed(const Config &config) : r(GetGPIOConfig(config, rgb_pins[(config.id) * 3])),
									   g(GetGPIOConfig(config, rgb_pins[(config.id) * 3 + 1])),
									   b(GetGPIOConfig(config, rgb_pins[(config.id) * 3 + 2])),
									   is_active_low(config.is_active_low)
{
}

void RGBLed::SetColor(const Color &color)
{
	switch (color)
	{
	case RGBLed::Color::kWhite:
		r.Set(!is_active_low);
		g.Set(!is_active_low);
		b.Set(!is_active_low);
		break;
	case RGBLed::Color::kRed:
		r.Set(!is_active_low);
		g.Set(is_active_low);
		b.Set(is_active_low);
		break;
	case RGBLed::Color::kGreen:
		r.Set(is_active_low);
		g.Set(!is_active_low);
		b.Set(is_active_low);
		break;
	case RGBLed::Color::kBlue:
		r.Set(is_active_low);
		g.Set(is_active_low);
		b.Set(!is_active_low);
		break;
	case RGBLed::Color::kYellow:
		r.Set(!is_active_low);
		g.Set(!is_active_low);
		b.Set(is_active_low);
		break;
	case RGBLed::Color::kMagenta:
		r.Set(!is_active_low);
		g.Set(is_active_low);
		b.Set(!is_active_low);
		break;
	case RGBLed::Color::kCyan:
		r.Set(is_active_low);
		g.Set(!is_active_low);
		b.Set(!is_active_low);
		break;
	}
}

} // namespace DeviceDriver

#endif
