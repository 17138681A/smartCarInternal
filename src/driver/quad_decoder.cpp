/*
 * quad_decoder.cpp
 *
 *  Created on: 19 Dec 2018
 *      Author: lch19
 */

#include "driver/quad_decoder.h"
#include "system/clock.h"

namespace Driver
{

using namespace System;
static const clock_ip_name_t enc_clock[] = ENC_CLOCKS;

QuadDecoder::QuadDecoder(const Config config)
{
	/* Enable the clock. */
	System::CLOCK_EnableClock(enc_clock[(uint8_t)config.module + 1]);

	decoder_base = (ENC_Type *)(0x403C8000u + (uint8_t)config.module * 0x4000);

	/* ENC_CTRL. */
	uint32_t tmp16 = decoder_base->CTRL & (uint16_t)(~(ENC_CTRL_HIRQ_MASK | ENC_CTRL_XIRQ_MASK | ENC_CTRL_DIRQ_MASK | ENC_CTRL_CMPIRQ_MASK | ENC_CTRL_HIP_MASK | ENC_CTRL_HNE_MASK | ENC_CTRL_REV_MASK | ENC_CTRL_PH1_MASK | ENC_CTRL_XIP_MASK | ENC_CTRL_XNE_MASK | ENC_CTRL_WDE_MASK));
	/* For HOME trigger. */
	if (config.HOME_trigger_mode != Config::HOMETriggerMode::kDisabled)
	{
		tmp16 |= ENC_CTRL_HIP_MASK;
		if (config.HOME_trigger_mode == Config::HOMETriggerMode::kOnFallingEdge)
		{
			tmp16 |= ENC_CTRL_HNE_MASK;
		}
	}
	/* For encoder work mode. */
	if (config.enable_reverse_direction)
	{
		tmp16 |= ENC_CTRL_REV_MASK;
	}
	if (config.decoder_work_mode == Config::WorkMode::kSignalPhaseCountMode)
	{
		tmp16 |= ENC_CTRL_PH1_MASK;
	}
	/* For INDEX trigger. */
	if (config.INDEX_trigger_mode != Config::INDEXTriggerMode::kDisabled)
	{
		tmp16 |= ENC_CTRL_XIP_MASK;
		if (config.INDEX_trigger_mode == Config::INDEXTriggerMode::kOnFallingEdge)
		{
			tmp16 |= ENC_CTRL_XNE_MASK;
		}
	}
	/* Watchdog. */
	if (config.enable_watchdog)
	{
		tmp16 |= ENC_CTRL_WDE_MASK;
		decoder_base->WTR = config.watchdog_timeout_value; /* WDOG can be only available when the feature is enabled. */
	}
	decoder_base->CTRL = tmp16;

	/* ENC_FILT. */
	decoder_base->FILT = ENC_FILT_FILT_CNT(config.filter_count) | ENC_FILT_FILT_PER(config.filter_sample_period);

	/* ENC_CTRL2. */
	tmp16 = decoder_base->CTRL2 & (uint16_t)(~(ENC_CTRL2_SABIRQ_MASK | ENC_CTRL2_ROIRQ_MASK | ENC_CTRL2_RUIRQ_MASK | ENC_CTRL2_OUTCTL_MASK | ENC_CTRL2_REVMOD_MASK |
											   ENC_CTRL2_MOD_MASK | ENC_CTRL2_UPDPOS_MASK | ENC_CTRL2_UPDHLD_MASK));
	if (config.position_match_mode == Config::PositionMatchMode::kOnReadingAnyPositionCounter)
	{
		tmp16 |= ENC_CTRL2_OUTCTL_MASK;
	}
	if (config.revolution_count_condition == Config::RevolutionCountCondition::kCountOnRollOverModulus)
	{
		tmp16 |= ENC_CTRL2_REVMOD_MASK;
	}
	if (config.enable_modulo_count_mode)
	{
		tmp16 |= ENC_CTRL2_MOD_MASK;
		/* Set modulus value. */
		decoder_base->UMOD = (uint16_t)(config.position_modulus_value >> 16U); /* Upper 16 bits. */
		decoder_base->LMOD = (uint16_t)(config.position_modulus_value);		   /* Lower 16 bits. */
	}
	if (config.enable_TRIGGER_clear_position_counter)
	{
		tmp16 |= ENC_CTRL2_UPDPOS_MASK;
	}
	if (config.enable_TRIGGER_clear_hold_position_counter)
	{
		tmp16 |= ENC_CTRL2_UPDHLD_MASK;
	}
	decoder_base->CTRL2 = tmp16;

	/* ENC_UCOMP & ENC_LCOMP. */
	decoder_base->UCOMP = (uint16_t)(config.position_compare_value >> 16U); /* Upper 16 bits. */
	decoder_base->LCOMP = (uint16_t)(config.position_compare_value);		/* Lower 16 bits. */

	/* ENC_UINIT & ENC_LINIT. */
	decoder_base->UINIT = (uint16_t)(config.position_initial_value >> 16U); /* Upper 16 bits. */
	decoder_base->LINIT = (uint16_t)(config.position_initial_value);		/* Lower 16 bits. */
}

void QuadDecoder::LoadInitialPositionValue()
{
	uint16_t tmp16 = decoder_base->CTRL & (uint16_t)(~(ENC_CTRL_HIRQ_MASK | ENC_CTRL_XIRQ_MASK | ENC_CTRL_DIRQ_MASK | ENC_CTRL_CMPIRQ_MASK));

	tmp16 |= ENC_CTRL_SWIP_MASK; /* Write 1 to trigger the command for loading initial position value. */
	decoder_base->CTRL = tmp16;
}

} // namespace Driver
