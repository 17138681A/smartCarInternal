/*
 * gpio.cpp
 *
 *  Created on: Aug 4, 2018
 *      Author: LeeChunHei
 */

#include "driver/gpio.h"
#include <assert.h>

namespace Driver
{

std::vector<GPIO *> port_listener[5];

GPIO::GPIO(const GPIO::Config &config)
{
	System::Pinout::Config pin_config;
	pin_config.force_input = config.force_input;
	pin_config.pin = config.pin;
	pin_config.mux_mode = System::Pinout::Config::MuxMode::kAlt5;
	pin_config.pin_config = config.pin_config;
	System::Pinout::InitPin(pin_config);
	pin_name = config.pin;
	if (config.pin <= System::Pinout::Name::kGPIO_EMC_31)
	{
		gpio_base = GPIO4;
		gpio_pin = (uint8_t)config.pin;
		System::CLOCK_EnableClock(System::kCLOCK_Gpio4);
	}
	else if (config.pin <= System::Pinout::Name::kGPIO_EMC_41)
	{
		gpio_base = GPIO3;
		gpio_pin = ((uint8_t)config.pin - (uint8_t)System::Pinout::Name::kGPIO_EMC_32 + 18);
		System::CLOCK_EnableClock(System::kCLOCK_Gpio3);
	}
	else if (config.pin <= System::Pinout::Name::kGPIO_AD_B1_15)
	{
		gpio_base = GPIO1;
		gpio_pin = ((uint8_t)config.pin - (uint8_t)System::Pinout::Name::kGPIO_AD_B0_00);
		System::CLOCK_EnableClock(System::kCLOCK_Gpio1);
	}
	else if (config.pin <= System::Pinout::Name::kGPIO_B1_15)
	{
		gpio_base = GPIO2;
		gpio_pin = ((uint8_t)config.pin - (uint8_t)System::Pinout::Name::kGPIO_B0_00);
		System::CLOCK_EnableClock(System::kCLOCK_Gpio2);
	}
	else if (config.pin <= System::Pinout::Name::kGPIO_SD_B0_05)
	{
		gpio_base = GPIO3;
		gpio_pin = ((uint8_t)config.pin - (uint8_t)System::Pinout::Name::kGPIO_SD_B0_00 + 12);
		System::CLOCK_EnableClock(System::kCLOCK_Gpio3);
	}
	else if (config.pin <= System::Pinout::Name::kGPIO_SD_B1_11)
	{
		gpio_base = GPIO3;
		gpio_pin = ((uint8_t)config.pin - (uint8_t)System::Pinout::Name::kGPIO_SD_B1_00);
		System::CLOCK_EnableClock(System::kCLOCK_Gpio3);
	}
	else
	{
		return;
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
	Set(config.default_high);
	if (config.interrupt_mode != Config::Interrupt::kDisable)
	{
		gpio_base->EDGE_SEL &= ~(1u << gpio_pin);
		volatile uint32_t *icr;
		uint32_t icr_shift = gpio_pin;
		if (gpio_pin < 16)
		{
			icr = &(gpio_base->ICR1);
		}
		else
		{
			icr = &(gpio_base->ICR2);
			icr_shift -= 16;
		}
		listener = config.listener;
		if (gpio_base == GPIO1)
		{
			port_listener[0].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO1_Combined_0_15_IRQn, config.interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO1_Combined_16_31_IRQn, config.interrupt_priority);
			}
		}
		else if (gpio_base == GPIO2)
		{
			port_listener[1].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO2_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO2_Combined_0_15_IRQn, config.interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO2_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO2_Combined_16_31_IRQn, config.interrupt_priority);
			}
		}
		else if (gpio_base == GPIO3)
		{
			port_listener[2].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO3_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO3_Combined_0_15_IRQn, config.interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO3_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO3_Combined_16_31_IRQn, config.interrupt_priority);
			}
		}
		else if (gpio_base == GPIO4)
		{
			port_listener[3].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO4_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO4_Combined_0_15_IRQn, config.interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO4_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO4_Combined_16_31_IRQn, config.interrupt_priority);
			}
		}
		else if (gpio_base == GPIO5)
		{
			port_listener[4].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO5_Combined_0_15_IRQn, config.interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO5_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO5_Combined_16_31_IRQn, config.interrupt_priority);
			}
		}
		switch (config.interrupt_mode)
		{
		case Config::Interrupt::kLow:
			*icr &= ~(3u << (2 * icr_shift));
			break;
		case Config::Interrupt::kHigh:
			*icr = (*icr & (~(3u << (2 * icr_shift)))) | (1u << (2 * icr_shift));
			break;
		case Config::Interrupt::kRising:
			*icr = (*icr & (~(3u << (2 * icr_shift)))) | (2u << (2 * icr_shift));
			break;
		case Config::Interrupt::kFalling:
			*icr |= (3u << (2 * icr_shift));
			break;
		case Config::Interrupt::kBoth:
			gpio_base->EDGE_SEL |= (1u << gpio_pin);
			break;
		default:
			break;
		}
		if (config.start_interrupt)
		{
			EnableInterrupt();
		}
	}
}

GPIO::~GPIO()
{
	if (listener)
	{
		int index = ((uint32_t)gpio_base - 0x401B8000) / 0x4000;
		for (uint8_t i = 0; i < port_listener[index].size(); i++)
		{
			if (port_listener[index][i]->GetPin() == gpio_pin)
			{
				port_listener[index].erase(port_listener[index].begin() + i);
				break;
			}
		}
	}
	System::Pinout::DeinitPin(pin_name);
}

GPIO::Direction GPIO::ToggleDirection(GPIO::Config::Interrupt interrupt_mode, uint8_t interrupt_priority, GPIO::GPIO_Listener listener, bool start_interrupt)
{
	if (gpio_base->GDIR >> gpio_pin & 1u)
	{
		gpio_base->GDIR &= ~(1u << gpio_pin);
		if (interrupt_mode != Config::Interrupt::kDisable)
		{
			gpio_base->EDGE_SEL &= ~(1u << gpio_pin);
			volatile uint32_t *icr;
			uint32_t icr_shift = gpio_pin;
			if (gpio_pin < 16)
			{
				icr = &(gpio_base->ICR1);
			}
			else
			{
				icr = &(gpio_base->ICR2);
				icr_shift -= 16;
			}
			this->listener = listener;
			if (gpio_base == GPIO1)
			{
				port_listener[0].push_back(this);
				if (gpio_pin < 16)
				{
					NVIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
					NVIC_SetPriority(GPIO1_Combined_0_15_IRQn, interrupt_priority);
				}
				else
				{
					NVIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
					NVIC_SetPriority(GPIO1_Combined_16_31_IRQn, interrupt_priority);
				}
			}
			else if (gpio_base == GPIO2)
			{
				port_listener[1].push_back(this);
				if (gpio_pin < 16)
				{
					NVIC_EnableIRQ(GPIO2_Combined_0_15_IRQn);
					NVIC_SetPriority(GPIO2_Combined_0_15_IRQn, interrupt_priority);
				}
				else
				{
					NVIC_EnableIRQ(GPIO2_Combined_16_31_IRQn);
					NVIC_SetPriority(GPIO2_Combined_16_31_IRQn, interrupt_priority);
				}
			}
			else if (gpio_base == GPIO3)
			{
				port_listener[2].push_back(this);
				if (gpio_pin < 16)
				{
					NVIC_EnableIRQ(GPIO3_Combined_0_15_IRQn);
					NVIC_SetPriority(GPIO3_Combined_0_15_IRQn, interrupt_priority);
				}
				else
				{
					NVIC_EnableIRQ(GPIO3_Combined_16_31_IRQn);
					NVIC_SetPriority(GPIO3_Combined_16_31_IRQn, interrupt_priority);
				}
			}
			else if (gpio_base == GPIO4)
			{
				port_listener[3].push_back(this);
				if (gpio_pin < 16)
				{
					NVIC_EnableIRQ(GPIO4_Combined_0_15_IRQn);
					NVIC_SetPriority(GPIO4_Combined_0_15_IRQn, interrupt_priority);
				}
				else
				{
					NVIC_EnableIRQ(GPIO4_Combined_16_31_IRQn);
					NVIC_SetPriority(GPIO4_Combined_16_31_IRQn, interrupt_priority);
				}
			}
			else if (gpio_base == GPIO5)
			{
				port_listener[4].push_back(this);
				if (gpio_pin < 16)
				{
					NVIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);
					NVIC_SetPriority(GPIO5_Combined_0_15_IRQn, interrupt_priority);
				}
				else
				{
					NVIC_EnableIRQ(GPIO5_Combined_16_31_IRQn);
					NVIC_SetPriority(GPIO5_Combined_16_31_IRQn, interrupt_priority);
				}
			}
			switch (interrupt_mode)
			{
			case Config::Interrupt::kLow:
				*icr &= ~(3u << (2 * icr_shift));
				break;
			case Config::Interrupt::kHigh:
				*icr = (*icr & (~(3u << (2 * icr_shift)))) | (1u << (2 * icr_shift));
				break;
			case Config::Interrupt::kRising:
				*icr = (*icr & (~(3u << (2 * icr_shift)))) | (2u << (2 * icr_shift));
				break;
			case Config::Interrupt::kFalling:
				*icr |= (3u << (2 * icr_shift));
				break;
			case Config::Interrupt::kBoth:
				gpio_base->EDGE_SEL |= (1u << gpio_pin);
				break;
			default:
				break;
			}
			if (start_interrupt)
			{
				EnableInterrupt();
			}
		}
		return Direction::kDigitalInput;
	}
	else
	{
		gpio_base->GDIR |= (1u << gpio_pin);
		if (gpio_base == GPIO1)
		{
			for (uint8_t i = 0; i < port_listener[0].size(); i++)
			{
				if (port_listener[0][i]->GetPin() == gpio_pin)
				{
					port_listener[0].erase(port_listener[0].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO2)
		{
			for (uint8_t i = 0; i < port_listener[1].size(); i++)
			{
				if (port_listener[1][i]->GetPin() == gpio_pin)
				{
					port_listener[1].erase(port_listener[1].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO3)
		{
			for (uint8_t i = 0; i < port_listener[2].size(); i++)
			{
				if (port_listener[2][i]->GetPin() == gpio_pin)
				{
					port_listener[2].erase(port_listener[2].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO4)
		{
			for (uint8_t i = 0; i < port_listener[3].size(); i++)
			{
				if (port_listener[3][i]->GetPin() == gpio_pin)
				{
					port_listener[3].erase(port_listener[3].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO5)
		{
			for (uint8_t i = 0; i < port_listener[4].size(); i++)
			{
				if (port_listener[4][i]->GetPin() == gpio_pin)
				{
					port_listener[4].erase(port_listener[4].begin() + i);
					break;
				}
			}
		}
		return Direction::kDigitalOutput;
	}
}

void GPIO::SetListener(Config::Interrupt interrupt_mode, GPIO_Listener listener, uint8_t interrupt_priority, bool start_interrupt)
{
	assert(listener);
	if (this->listener)
	{
		if (gpio_base == GPIO1)
		{
			for (uint8_t i = 0; i < port_listener[0].size(); i++)
			{
				if (port_listener[0][i]->GetPin() == gpio_pin)
				{
					port_listener[0].erase(port_listener[0].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO2)
		{
			for (uint8_t i = 0; i < port_listener[1].size(); i++)
			{
				if (port_listener[1][i]->GetPin() == gpio_pin)
				{
					port_listener[1].erase(port_listener[1].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO3)
		{
			for (uint8_t i = 0; i < port_listener[2].size(); i++)
			{
				if (port_listener[2][i]->GetPin() == gpio_pin)
				{
					port_listener[2].erase(port_listener[2].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO4)
		{
			for (uint8_t i = 0; i < port_listener[3].size(); i++)
			{
				if (port_listener[3][i]->GetPin() == gpio_pin)
				{
					port_listener[3].erase(port_listener[3].begin() + i);
					break;
				}
			}
		}
		else if (gpio_base == GPIO5)
		{
			for (uint8_t i = 0; i < port_listener[4].size(); i++)
			{
				if (port_listener[4][i]->GetPin() == gpio_pin)
				{
					port_listener[4].erase(port_listener[4].begin() + i);
					break;
				}
			}
		}
	}
	if (interrupt_mode != Config::Interrupt::kDisable)
	{
		gpio_base->EDGE_SEL &= ~(1u << gpio_pin);
		volatile uint32_t *icr;
		uint32_t icr_shift = gpio_pin;
		if (gpio_pin < 16)
		{
			icr = &(gpio_base->ICR1);
		}
		else
		{
			icr = &(gpio_base->ICR2);
			icr_shift -= 16;
		}
		this->listener = listener;
		if (gpio_base == GPIO1)
		{
			port_listener[0].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO1_Combined_0_15_IRQn, interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO1_Combined_16_31_IRQn, interrupt_priority);
			}
		}
		else if (gpio_base == GPIO2)
		{
			port_listener[1].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO2_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO2_Combined_0_15_IRQn, interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO2_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO2_Combined_16_31_IRQn, interrupt_priority);
			}
		}
		else if (gpio_base == GPIO3)
		{
			port_listener[2].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO3_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO3_Combined_0_15_IRQn, interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO3_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO3_Combined_16_31_IRQn, interrupt_priority);
			}
		}
		else if (gpio_base == GPIO4)
		{
			port_listener[3].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO4_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO4_Combined_0_15_IRQn, interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO4_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO4_Combined_16_31_IRQn, interrupt_priority);
			}
		}
		else if (gpio_base == GPIO5)
		{
			port_listener[4].push_back(this);
			if (gpio_pin < 16)
			{
				NVIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);
				NVIC_SetPriority(GPIO5_Combined_0_15_IRQn, interrupt_priority);
			}
			else
			{
				NVIC_EnableIRQ(GPIO5_Combined_16_31_IRQn);
				NVIC_SetPriority(GPIO5_Combined_16_31_IRQn, interrupt_priority);
			}
		}
		switch (interrupt_mode)
		{
		case Config::Interrupt::kLow:
			*icr &= ~(3u << (2 * icr_shift));
			break;
		case Config::Interrupt::kHigh:
			*icr = (*icr & (~(3u << (2 * icr_shift)))) | (1u << (2 * icr_shift));
			break;
		case Config::Interrupt::kRising:
			*icr = (*icr & (~(3u << (2 * icr_shift)))) | (2u << (2 * icr_shift));
			break;
		case Config::Interrupt::kFalling:
			*icr |= (3u << (2 * icr_shift));
			break;
		case Config::Interrupt::kBoth:
			gpio_base->EDGE_SEL |= (1u << gpio_pin);
			break;
		default:
			break;
		}
		if (start_interrupt)
		{
			EnableInterrupt();
		}
	}
}

#if defined(__cplusplus)
extern "C"
{
#endif

	void GPIO1_INT0_IRQHandler()
	{
	}
	void GPIO1_INT1_IRQHandler()
	{
	}
	void GPIO1_INT2_IRQHandler()
	{
	}
	void GPIO1_INT3_IRQHandler()
	{
	}
	void GPIO1_INT4_IRQHandler()
	{
	}
	void GPIO1_INT5_IRQHandler()
	{
	}
	void GPIO1_INT6_IRQHandler()
	{
	}
	void GPIO1_INT7_IRQHandler()
	{
	}
	void GPIO1_Combined_0_15_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[0].size(); i++)
		{
			if (((GPIO1->ISR >> (port_listener[0][i]->GetPin())) & 0x1u))
			{
				port_listener[0][i]->GetListener()(port_listener[0][i]);
				GPIO1->ISR |= 1u << port_listener[0][i]->GetPin();
				if (!GPIO1->ISR)
					break;
			}
		}
	}
	void GPIO1_Combined_16_31_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[0].size(); i++)
		{
			if (((GPIO1->ISR >> (port_listener[0][i]->GetPin())) & 0x1u))
			{
				port_listener[0][i]->GetListener()(port_listener[0][i]);
				GPIO1->ISR |= 1u << port_listener[0][i]->GetPin();
				if (!GPIO1->ISR)
					break;
			}
		}
	}
	void GPIO2_Combined_0_15_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[1].size(); i++)
		{
			if (((GPIO2->ISR >> (port_listener[1][i]->GetPin())) & 0x1u))
			{
				port_listener[1][i]->GetListener()(port_listener[1][i]);
				GPIO2->ISR |= 1u << port_listener[1][i]->GetPin();
				if (!GPIO2->ISR)
					break;
			}
		}
	}
	void GPIO2_Combined_16_31_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[1].size(); i++)
		{
			if (((GPIO2->ISR >> (port_listener[1][i]->GetPin())) & 0x1u))
			{
				port_listener[1][i]->GetListener()(port_listener[1][i]);
				GPIO2->ISR |= 1u << port_listener[1][i]->GetPin();
				if (!GPIO2->ISR)
					break;
			}
		}
	}
	void GPIO3_Combined_0_15_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[2].size(); i++)
		{
			if (((GPIO3->ISR >> (port_listener[2][i]->GetPin())) & 0x1u))
			{
				port_listener[2][i]->GetListener()(port_listener[2][i]);
				GPIO3->ISR |= 1u << port_listener[2][i]->GetPin();
				if (!GPIO3->ISR)
					break;
			}
		}
	}
	void GPIO3_Combined_16_31_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[2].size(); i++)
		{
			if (((GPIO3->ISR >> (port_listener[2][i]->GetPin())) & 0x1u))
			{
				port_listener[2][i]->GetListener()(port_listener[2][i]);
				GPIO3->ISR |= 1u << port_listener[2][i]->GetPin();
				if (!GPIO3->ISR)
					break;
			}
		}
	}
	void GPIO4_Combined_0_15_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[3].size(); i++)
		{
			if (((GPIO4->ISR >> (port_listener[3][i]->GetPin())) & 0x1u))
			{
				port_listener[3][i]->GetListener()(port_listener[3][i]);
				GPIO4->ISR |= 1u << port_listener[3][i]->GetPin();
				if (!GPIO4->ISR)
					break;
			}
		}
	}
	void GPIO4_Combined_16_31_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[3].size(); i++)
		{
			if (((GPIO4->ISR >> (port_listener[3][i]->GetPin())) & 0x1u))
			{
				port_listener[3][i]->GetListener()(port_listener[3][i]);
				GPIO4->ISR |= 1u << port_listener[3][i]->GetPin();
				if (!GPIO4->ISR)
					break;
			}
		}
	}
	void GPIO5_Combined_0_15_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[4].size(); i++)
		{
			if (((GPIO5->ISR >> (port_listener[4][i]->GetPin())) & 0x1u))
			{
				port_listener[4][i]->GetListener()(port_listener[4][i]);
				GPIO5->ISR |= 1u << port_listener[4][i]->GetPin();
				if (!GPIO5->ISR)
					break;
			}
		}
	}
	void GPIO5_Combined_16_31_IRQHandler()
	{
		for (uint8_t i = 0; i < port_listener[4].size(); i++)
		{
			if (((GPIO5->ISR >> (port_listener[4][i]->GetPin())) & 0x1u))
			{
				port_listener[4][i]->GetListener()(port_listener[4][i]);
				GPIO5->ISR |= 1u << port_listener[4][i]->GetPin();
				if (!GPIO5->ISR)
					break;
			}
		}
	}

#if defined(__cplusplus)
}
#endif

} // namespace Driver
