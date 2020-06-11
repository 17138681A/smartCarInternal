/*
 * gpio_array.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: LeeChunHei
 */

#include "driver/gpio_array.h"
#include "system/clock.h"

namespace Driver
{

GPIOArray::GPIOArray(Config &config)
{
	System::Pinout::Config pin_config;
	pin_config.mux_mode = System::Pinout::Config::MuxMode::kAlt5;
	pin_config.pin_config = config.pin_config;
	pin_config.force_input = true;
	for (uint32_t i = 0; i < config.pin.size(); i++)
	{
		pin_config.pin = config.pin[i];
		System::Pinout::InitPin(pin_config);
		GPIO_Type *gpio_base = nullptr;
		uint8_t gpio_pin;
		if (config.pin[i] <= System::Pinout::Name::kGPIO_EMC_31)
		{
			gpio_base = GPIO4;
			gpio_pin = (uint8_t)config.pin[i];
			System::CLOCK_EnableClock(System::kCLOCK_Gpio4);
		}
		else if (config.pin[i] <= System::Pinout::Name::kGPIO_EMC_41)
		{
			gpio_base = GPIO3;
			gpio_pin = ((uint8_t)config.pin[i] - (uint8_t)System::Pinout::Name::kGPIO_EMC_32 + 18);
			System::CLOCK_EnableClock(System::kCLOCK_Gpio3);
		}
		else if (config.pin[i] <= System::Pinout::Name::kGPIO_AD_B1_15)
		{
			gpio_base = GPIO1;
			gpio_pin = ((uint8_t)config.pin[i] - (uint8_t)System::Pinout::Name::kGPIO_AD_B0_00);
			System::CLOCK_EnableClock(System::kCLOCK_Gpio1);
		}
		else if (config.pin[i] <= System::Pinout::Name::kGPIO_B1_15)
		{
			gpio_base = GPIO2;
			gpio_pin = ((uint8_t)config.pin[i] - (uint8_t)System::Pinout::Name::kGPIO_B0_00);
			System::CLOCK_EnableClock(System::kCLOCK_Gpio2);
		}
		else if (config.pin[i] <= System::Pinout::Name::kGPIO_SD_B0_05)
		{
			gpio_base = GPIO3;
			gpio_pin = ((uint8_t)config.pin[i] - (uint8_t)System::Pinout::Name::kGPIO_SD_B0_00 + 12);
			System::CLOCK_EnableClock(System::kCLOCK_Gpio3);
		}
		else if (config.pin[i] <= System::Pinout::Name::kGPIO_SD_B1_11)
		{
			gpio_base = GPIO3;
			gpio_pin = ((uint8_t)config.pin[i] - (uint8_t)System::Pinout::Name::kGPIO_SD_B1_00);
			System::CLOCK_EnableClock(System::kCLOCK_Gpio3);
		}
		gpio_base->IMR &= ~(1u << gpio_pin);
		if (config.gpio_dir == Direction::kDigitalInput)
		{
			gpio_base->GDIR &= ~(1u << gpio_pin);
		}
		else
		{
			gpio_base->GDIR |= (1u << gpio_pin);
		}
		if (pin_info.size())
		{
			if (gpio_base == pin_info.back().gpio_base)
			{
				pin_info.back().gpio_pin.push_back(gpio_pin);
			}
			else
			{
				PinInfo pin_info_temp;
				pin_info_temp.gpio_base = gpio_base;
				pin_info_temp.gpio_pin.push_back(gpio_pin);
				pin_info.push_back(pin_info_temp);
			}
		}
		else
		{
			PinInfo pin_info_temp;
			pin_info_temp.gpio_base = gpio_base;
			pin_info_temp.gpio_pin.push_back(gpio_pin);
			pin_info.push_back(pin_info_temp);
		}
	}
	if (pin_info.size() == 1)
	{
		shift = pin_info.back().gpio_pin[0];
		mask = 1;
		for (uint32_t i = 1; i < pin_info.back().gpio_pin.size(); i++)
		{
			if ((pin_info.back().gpio_pin[i] - pin_info.back().gpio_pin[i - 1]) == 1)
			{
				mask |= (1 << i);
			}
			else
			{
				mask = 0;
				break;
			}
		}
		if (mask)
		{
			mask = ~(mask << shift);
			had_mask = true;
		}
	}
}

void GPIOArray::Set(uint32_t output)
{
	if (had_mask)
	{
		uint32_t dr = pin_info[0].gpio_base->DR & mask;
		dr |= (output << shift);
		pin_info[0].gpio_base->DR = dr;
	}
	else
	{
		uint8_t shift_count = 0;
		for (uint8_t i = 0; i < pin_info.size(); i++)
		{
			uint32_t dr = pin_info[i].gpio_base->DR;
			for (uint8_t j = 0; j < pin_info[i].gpio_pin.size(); j++)
			{
				if ((output >> shift_count++) & 1)
				{
					dr |= (1u << pin_info[i].gpio_pin[j]);
				}
				else
				{
					dr &= ~(1u << pin_info[i].gpio_pin[j]);
				}
			}
			pin_info[i].gpio_base->DR = dr;
		}
	}
}

void GPIOArray::Toggle()
{
	if (had_mask)
	{
		pin_info[0].gpio_base->DR_TOGGLE = ~(mask);
	}
	else
	{
		for (uint8_t i = 0; i < pin_info.size(); i++)
		{
			uint32_t toggle_reg = 0;
			for (uint8_t j = 0; j < pin_info[i].gpio_pin.size(); j++)
			{
				toggle_reg |= 1u << pin_info[i].gpio_pin[j];
			}
			pin_info[i].gpio_base->DR = toggle_reg;
		}
	}
}

uint32_t GPIOArray::Get() const
{
	if (had_mask)
	{
		return (pin_info[0].gpio_base->DR & (~mask)) >> shift;
	}
	else
	{
		uint32_t data = 0;
		uint8_t shift_count = 0;
		for (uint8_t i = 0; i < pin_info.size(); i++)
		{
			uint32_t dr = pin_info[i].gpio_base->DR;
			for (uint8_t j = 0; j < pin_info[i].gpio_pin.size(); j++)
			{
				if ((dr >> pin_info[i].gpio_pin[j]) & 1)
				{
					data |= (1u << shift_count++);
				}
				else
				{
					data &= ~(1u << shift_count++);
				}
			}
		}
		return data;
	}
}

} // namespace Driver
