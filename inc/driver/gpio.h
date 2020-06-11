/*
 * gpio.h
 *
 *  Created on: Aug 4, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_GPIO_H_
#define INC_DRIVER_GPIO_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"
#include "system/clock.h"
#include <vector>
#include <functional>

namespace Driver
{

/*!
 * @addtogroup gpio General Purpose Input/Output
 * @ingroup driver
 * The GPIO general-purpose input/output peripheral provides dedicated general-purpose pins that can be configured as either inputs or outputs
 * @{
 */
/**
 * @brief General Purpose Input/Output
 */
class GPIO
{
  public:
	typedef std::function<void(GPIO *gpio)> GPIO_Listener;
	enum struct Direction
	{
		kDigitalInput,
		kDigitalOutput
	};
	struct Config
	{
		enum struct Interrupt
		{
			kDisable = 0U, //No interrupt
			kLow = 1U,	 //Low level interrupt
			kHigh = 2U,	//High level interrupt
			kRising = 3U,  //Rising edge interrupt
			kFalling = 4U, //Falling edge interrupt
			kBoth = 5U,	//Rising edge and falling edge interrupt
		};
		System::Pinout::Name pin;
		System::Pinout::Config::PinConfig pin_config;
		Direction gpio_dir;
		bool default_high = false;
		Interrupt interrupt_mode = Interrupt::kDisable;
		uint8_t interrupt_priority = 15; //Interrupt priority, range [0-15], smaller value means higher priority
		GPIO_Listener listener = nullptr;
		bool start_interrupt = false;
		bool force_input = false;
	};
	GPIO(const Config &config);
	~GPIO();
	/*
	 * @brief Set the output logic level, do nothing when it is a digital input
	 *
	 * @param output_level: pin logic level.
	 */
	void Set(bool output_level)
	{
		if (output_level)
		{
			gpio_base->DR |= (1u << gpio_pin);
		}
		else
		{
			gpio_base->DR &= ~(1u << gpio_pin);
		}
	}
	/*
	 * @brief Toggle the output logic level, do nothing when it is a digital input.
	 *
	 * @retval final logic level.
	 */
	bool Toggle()
	{
		gpio_base->DR_TOGGLE = (1u << gpio_pin);
		return (((gpio_base->DR) >> gpio_pin) & 1u);
	}
	/*
	 * @brief Get the input logic level.
	 *
	 * @retval input logic level.
	 */
	bool Get() const
	{
		return (((gpio_base->PSR) >> gpio_pin) & 1u);
	}
	/*
	 * @brief Enable pin interrupt
	 */
	void EnableInterrupt()
	{
		gpio_base->IMR |= (1u << gpio_pin);
	}
	/*
	 * @brief Disable pin interrupt
	 */
	void DisableInterrupt()
	{
		gpio_base->IMR &= ~(1u << gpio_pin);
	}
	/*
	 * @brief Toggle the gpio direction, i.e. change from gpo to gpi.
	 *
	 * @retval final gpio mode.
	 */
	Direction ToggleDirection(Config::Interrupt interrupt_mode = Config::Interrupt::kDisable, uint8_t interrupt_priority = 15, GPIO_Listener listener = nullptr, bool start_interrupt = false);
	/*
	 * @brief Get the port base pointer
	 *
	 * @retval corresponding gpio_base pointer
	 */
	GPIO_Type *GetBase() const
	{
		return gpio_base;
	}
	/*
	 * @brief Get the pin number in the port
	 *
	 * @retval gpio_pin
	 */
	uint8_t GetPin() const
	{
		return gpio_pin;
	}
	/*
	 * @brief Get the pin interrupt listener
	 *
	 * @retval pin's listener
	 */
	GPIO_Listener GetListener()
	{
		return listener;
	}
	/*
	 * @brief Set the pin interrupt listener
	 *
	 * @param interrupt_mode: in what state the interrupt will occur
	 * 		  listener: interrupt listener
	 * 		  start_interrupt: true->interrupt enable
	 * 		                   false->interrupt disable
	 *
	 */
	void SetListener(Config::Interrupt interrupt_mode, GPIO_Listener listener, uint8_t interrupt_priority = 15, bool start_interrupt = true);

  private:
	GPIO_Type *gpio_base;
	uint8_t gpio_pin;
	System::Pinout::Name pin_name;
	GPIO_Listener listener;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_GPIO_H_ */
