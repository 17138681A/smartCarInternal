/*
 * adc.h
 *
 *  Created on: Aug 23, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_ADC_H_
#define INC_DRIVER_ADC_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/MIMXRT1052.h"
#include <bitset>

/*!
 * @addtogroup driver Driver
 * Peripheral library
 * @{
 */
namespace Driver
{

/*!
 * @addtogroup adc Analog-to-Digital Converter
 * @ingroup driver
 * The analog-to-digital converter (ADC) is a successive approximation ADC designed for operation within an integrated microcontroller system-on-chip
 * @{
 */
/**
 * @brief Analog-to-Digital Converter
 */
class ADC
{
  public:
	struct Config
	{
		enum struct ADCModule
		{
			kADC1,
			kADC2
		};
		enum struct SamplePeriodMode
		{
			/* For long sample mode. */
			kLongSamplePeriod12Clcoks = 0u, /*!< Long sample 12 clocks. */
			kLongSamplePeriod16Clcoks = 1u, /*!< Long sample 16 clocks. */
			kLongSamplePeriod20Clcoks = 2u, /*!< Long sample 20 clocks. */
			kLongSamplePeriod24Clcoks = 3u, /*!< Long sample 24 clocks. */
			/* For short sample mode. */
			kShortSample2Clocks = 0u, /*!< Short sample 2 clocks. */
			kShortSample4Clocks = 1u, /*!< Short sample 4 clocks. */
			kShortSample6Clocks = 2u, /*!< Short sample 6 clocks. */
			kShortSample8Clocks = 3u, /*!< Short sample 8 clocks. */
		};
		enum struct ClockSource
		{
			kIPGClock,  /*!< Select IPG clock to generate ADCK. */
			kIPGDiv2,   /*!< Select IPG clock divided by 2 to generate ADCK. */
			kADCK = 3u, /*!< Select Asynchronous clock to generate ADCK. */
		};
		enum struct ClockDiv
		{
			kDiv1 = 0U, /*!< For divider 1 from the input clock to the module. */
			kDiv2 = 1U, /*!< For divider 2 from the input clock to the module. */
			kDiv4 = 2U, /*!< For divider 4 from the input clock to the module. */
			kDiv8 = 3U, /*!< For divider 8 from the input clock to the module. */
		};
		enum struct Resolution
		{
			k8Bit = 0U,  /*!< Single End 8-bit resolution. */
			k10Bit = 1U, /*!< Single End 10-bit resolution. */
			k12Bit = 2U, /*!< Single End 12-bit resolution. */
		};
		ADCModule adc_module;
		bool enable_overwrite = false;												 /*!< Enable the overwriting. */
		bool enable_continuous_conversion = false;									 /*!< Enable the continuous conversion mode. */
		bool enable_high_speed = false;												 /*!< Enable the high-speed mode. */
		bool enable_low_power = false;												 /*!< Enable the low power mode. */
		bool enable_long_sample = false;											 /*!< Enable the long sample mode. */
		bool enable_asynchronous_clock_output = true;								 /*!< Enable the asynchronous clock output. */
		SamplePeriodMode sample_period_mode = SamplePeriodMode::kShortSample2Clocks; /*!< Select the sample period in long sample mode or short mode. */
		ClockSource clock_source = ClockSource::kADCK;								 /*!< Select the input clock source to generate the internal clock ADCK. */
		ClockDiv clock_divider = ClockDiv::kDiv1;									 /*!< Select the divide ratio used by the ADC to generate the internal clock ADCK. */
		Resolution resolution = Resolution::k12Bit;									 /*!< Select the ADC resolution mode. */
	};
	ADC(Config &config);
	void ConfigHWTrigger(bool enable);
	bool AutoCalibration();
	enum struct Channel
	{
		kChannel0,
		kChannel1,
		kChannel2,
		kChannel3,
		kChannel4,
		kChannel5,
		kChannel6,
		kChannel7
	};
	enum struct Input
	{
		kInput0,
		kInput1,
		kInput2,
		kInput3,
		kInput4,
		kInput5,
		kInput6,
		kInput7,
		kInput8,
		kInput9,
		kInput10,
		kInput11,
		kInput12,
		kInput13,
		kInput14,
		kInput15
	};
	typedef void (*ADC_Listener)(ADC *adc);
	bool ConfigChannel(Channel channel, Input input, ADC_Listener listener = nullptr, uint8_t interrupt_priority = 15);
	inline void StartChannelConversation(Input input)
	{
	    adc_base->HC[0] &= ~0x1F;
		adc_base->HC[0] |= (uint8_t)input;
	}
	inline bool IsComplete(Channel channel) const
	{
		return ((adc_base->HS) & (1U << (uint8_t)channel)) >> (uint8_t)channel;
	}
	inline uint16_t GetValue(Channel channel) const
	{
		return adc_base->R[(uint8_t)channel];
	}
	ADC_Listener *GetListener() const
	{
		return listener;
	}
	inline void EnableDMA(bool enable)
	{
		if (enable)
		{
			adc_base->GC |= ADC_GC_DMAEN_MASK;
		}
		else
		{
			adc_base->GC &= ~ADC_GC_DMAEN_MASK;
		}
	}

  private:
	System::Pinout::Name GetInputPinName(Input input);
	bool OpenADCInputPin(System::Pinout::Name pin_name);
	void CloseADCInputPin(System::Pinout::Name pin_name);

	ADC_Type *adc_base;
	ADC_Listener *listener;
	std::bitset<16> opened_input_pin;
};
/** @}*/

} // namespace Driver
/** @}*/

#endif /* INC_DRIVER_ADC_H_ */
