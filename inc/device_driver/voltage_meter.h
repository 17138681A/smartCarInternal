/*
 * voltage_meter.h
 *
 *  Created on: 1 Dec 2018
 *      Author: lch19
 */

#ifndef SRC_DEVICE_DRIVER_VOLTAGE_METER_H_
#define SRC_DEVICE_DRIVER_VOLTAGE_METER_H_

#include "driver/adc.h"
#include "../config/config.h"

#ifdef VOLTAGE_METER_USED

namespace DeviceDriver
{

/*!
 * @addtogroup battery Battery
 * @ingroup device_driver
 * @{
 */
/**
 * @brief Voltage meter driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class VoltageMeter
{
  public:
	struct Config
	{
		uint8_t id = 0;
		/*
		 *
		 * (Voltage)----[ R1 ]--.--[ R2 ]----(GND)
		 * 					    |
		 * 				        |
		 * 				   (MCU Input)
		 *
		 * voltage_divider_ratio = R2 / ( R1 + R2 )
		 *
		 */
		float voltage_divider_ratio;
	};
	VoltageMeter(const Config& config);

	/**
	 * @brief Get the real voltage that measured
	 * 
	 * @return float measured real voltage
	 */
	inline float GetVoltage()
	{
		adc.StartChannelConversation(input);
		while (!adc.IsComplete(channel))
			;
		return voltage_divider_ratio * adc.GetValue(channel);
	}

  private:
	Driver::ADC adc;
	Driver::ADC::Channel channel;
	Driver::ADC::Input input;
	float voltage_divider_ratio;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* SRC_DEVICE_DRIVER_VOLTAGE_METER_H_ */
