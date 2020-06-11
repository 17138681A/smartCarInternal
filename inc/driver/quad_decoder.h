/*
 * quad_decoder.h
 *
 *  Created on: 30 Nov 2018
 *      Author: lch19
 */

#ifndef SRC_DRIVER_QUAD_DECODER_H_
#define SRC_DRIVER_QUAD_DECODER_H_

#include <cstdint>
#include "system/cmsis/access_layer/access_layer.h"

namespace Driver
{

/*!
 * @addtogroup qdecoder Quadrature Decoder
 * @ingroup driver
 * The enhanced quadrature encoder/decoder module interfaces to position/speed sensors that are used in industrial motor control applications. 
 * Using 5 input signals (PHASEA, PHASEB, INDEX, TRIGGER, and HOME) from those position/speed sensors, 
 * the quadrature decoder module decodes shaft position, revolution count, and speed.
 * @{
 */
/**
 * @brief Quadrature Decoder
 */
class QuadDecoder
{
  public:
	struct Config
	{
		enum struct Module
		{
			k1,
			k2,
			k3,
			k4
		};
		enum struct WorkMode
		{
			kNormalMode,		  /*!< Use standard quadrature decoder with PHASEA and PHASEB. */
			kSignalPhaseCountMode /*!< PHASEA input generates a count signal while PHASEB input control the
			 direction. */
		};
		enum struct HOMETriggerMode
		{
			kDisabled,	 /*!< HOME signal's trigger is disabled. */
			kOnRisingEdge, /*!< Use positive going edge-to-trigger initialization of position counters. */
			kOnFallingEdge /*!< Use negative going edge-to-trigger initialization of position counters. */
		};
		enum struct INDEXTriggerMode
		{
			kDisabled,		/*!< INDEX signal's trigger is disabled. */
			kOnRisingEdge,  /*!< Use positive going edge-to-trigger initialization of position counters. */
			kOnFallingEdge, /*!< Use negative going edge-to-trigger initialization of position counters. */
		};
		enum struct PositionMatchMode
		{
			kOnPositionCounterEqualToComapreValue = 0U, /*!< POSMATCH pulses when a match occurs between the
			 position counters (POS) and the compare value (COMP). */
			kOnReadingAnyPositionCounter,				/*!< POSMATCH pulses when any position counter register is read. */
		};
		enum struct RevolutionCountCondition
		{
			kCountOnINDEXPulse = 0U, /*!< Use INDEX pulse to increment/decrement revolution counter. */
			kCountOnRollOverModulus, /*!< Use modulus counting roll-over/under to increment/decrement revolution
			 counter. */
		};

		Module module;
		/* Basic counter. */
		bool enable_reverse_direction = false;				/*!< Enable reverse direction counting. */
		WorkMode decoder_work_mode = WorkMode::kNormalMode; /*!< Enable signal phase count mode. */

		/* Signal detection. */
		HOMETriggerMode HOME_trigger_mode = HOMETriggerMode::kDisabled;	/*!< Enable HOME to initialize position counters. */
		INDEXTriggerMode INDEX_trigger_mode = INDEXTriggerMode::kDisabled; /*!< Enable INDEX to initialize position counters. */
		bool enable_TRIGGER_clear_position_counter = false;				   /*!< Clear POSD, REV, UPOS and LPOS on rising edge of TRIGGER, or not. */
		bool enable_TRIGGER_clear_hold_position_counter = false;		   /*!< Enable update of hold registers on rising edge of TRIGGER, or not.
		 */

		/* Watchdog. */
		bool enable_watchdog = false;		 /*!< Enable the watchdog to detect if the target is moving or not. */
		uint16_t watchdog_timeout_value = 0; /*!< Watchdog timeout count value. It stores the timeout count for the quadrature
		 decoder module watchdog timer. This field is only available when
		 "enableWatchdog" = true. The available value is a 16-bit unsigned number.*/

		/* Filter for PHASEA, PHASEB, INDEX and HOME. */
		uint16_t filter_count = 0;		   /*!< Input Filter Sample Count. This value should be chosen to reduce the probability of
		 noisy samples causing an incorrect transition to be recognized. The value represent the
		 number of consecutive samples that must agree prior to the input filter accepting an
		 input transition. A value of 0x0 represents 3 samples. A value of 0x7 represents 10
		 samples. The Available range is 0 - 7.*/
		uint16_t filter_sample_period = 0; /*!< Input Filter Sample Period. This value should be set such that the sampling period
		 is larger than the period of the expected noise. This value represents the
		 sampling period (in IPBus clock cycles) of the decoder input signals.
		 The available range is 0 - 255. */

		/* Position compare. */
		PositionMatchMode position_match_mode = PositionMatchMode::kOnPositionCounterEqualToComapreValue; /*!< The condition of POSMATCH pulses. */
		uint32_t position_compare_value = 0xFFFFFFFF;													  /*!< Position compare value. The available value is a 32-bit number.*/

		/* Modulus counting. */
		RevolutionCountCondition revolution_count_condition = RevolutionCountCondition::kCountOnINDEXPulse; /*!< Revolution Counter Modulus Enable. */
		bool enable_modulo_count_mode = false;																/*!< Enable Modulo Counting. */
		uint32_t position_modulus_value = 0;																/*!< Position modulus value. This value would be available only when
		 "enableModuloCountMode" = true. The available value is a 32-bit number. */
		uint32_t position_initial_value = 0;																/*!< Position initial value. The available value is a 32-bit number. */
	};
	QuadDecoder(const Config config);

	void LoadInitialPositionValue();

	inline uint32_t GetPositionCount()
	{
		uint32_t count = decoder_base->UPOS; /* Get upper 16 bits and make a snapshot. */
		count <<= 16;
		count |= decoder_base->LPOSH; /* Get lower 16 bits from hold register. */
		return count;
	}

	inline uint32_t GetHoldPositionCount()
	{
		uint32_t count = decoder_base->UPOSH; /* Get upper 16 bits and make a snapshot. */
		count <<= 16;
		count |= decoder_base->LPOSH; /* Get lower 16 bits from hold register. */
		return count;
	}

	inline int16_t GetPositionDifferentValue()
	{
		return (int16_t)decoder_base->POSD;
	}

	inline int16_t GetHoldPositionDifferentValue()
	{
		return (int16_t)decoder_base->POSDH;
	}

	inline uint16_t GetRevolutionValue()
	{
		return decoder_base->REV;
	}

	inline uint16_t GetHoldRevolutionValue()
	{
		return decoder_base->REVH;
	}

  private:
	ENC_Type *decoder_base;
};
/** @}*/

} // namespace Driver

#endif /* SRC_DRIVER_QUAD_DECODER_H_ */
