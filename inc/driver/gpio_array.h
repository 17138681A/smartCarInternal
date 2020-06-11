/*
 * gpio_array.h
 *
 *  Created on: Sep 19, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_GPIO_ARRAY_H_
#define INC_DRIVER_GPIO_ARRAY_H_

#include "system/pinout/pinout.h"
#include <vector>

namespace Driver
{

/*!
 * @ingroup gpio
 * @{
 */
/**
 * @brief Array of GPIO that can control at the same time
 */
class GPIOArray
{
  public:
	enum struct Direction
	{
		kDigitalInput,
		kDigitalOutput
	};
	struct Config
	{
		std::vector<System::Pinout::Name> pin; //First pin is the LSB
		System::Pinout::Config::PinConfig pin_config;
		Direction gpio_dir;
		bool default_high = false;
	};
	GPIOArray(Config &config);

	/*
	 * @brief Set the output logic level, do nothing when it is a digital input
	 *
	 * @param output_level: pin logic level.
	 */
	void Set(uint32_t output);

	/*
	 * @brief Toggle the output logic level, do nothing when it is a digital input.
	 */
	void Toggle();

	/*
	 * @brief Get the input logic level.
	 *
	 * @retval input logic level.
	 */
	uint32_t Get() const;

  private:
	struct PinInfo
	{
		GPIO_Type *gpio_base;
		std::vector<uint8_t> gpio_pin;
	};
	std::vector<PinInfo> pin_info;
	uint32_t mask = 0;
	bool had_mask = false;
	uint8_t shift;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_GPIO_ARRAY_H_ */
