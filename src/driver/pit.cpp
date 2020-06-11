/*
 * pit.cpp
 *
 *  Created on: Oct 5, 2018
 *      Author: LeeChunHei
 */

#include "driver/pit.h"
#include "system/clock.h"
#include "system/system.h"

namespace Driver
{

using namespace System;
static const clock_ip_name_t s_pitClocks[] = PIT_CLOCKS;
uint32_t pit_clock = 0;
Pit *pit_ptr[4];

Pit::Pit(const Config &config) : listener(config.listener), channel(config.channel)
{
	/* Ungate the PIT clock*/
	CLOCK_EnableClock(s_pitClocks[0]);

	pit_base = (PIT_Type *)(0x40084000u);

	if (!pit_clock)
	{
		pit_clock = CLOCK_GetFreq(kCLOCK_IpgClk) / (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1);
	}

	/* Enable PIT timers */
	pit_base->MCR &= ~PIT_MCR_MDIS_MASK;
	/* Config timer operation when in debug mode */
	if (config.enable_debug_run)
	{
		pit_base->MCR &= ~PIT_MCR_FRZ_MASK;
	}
	else
	{
		pit_base->MCR |= PIT_MCR_FRZ_MASK;
	}

	ChangePeriodCount(config.period_count);

	if (config.listener)
	{
		pit_ptr[(uint8_t)channel] = this;
		pit_base->CHANNEL[(uint8_t)channel].TCTRL |= PIT_TCTRL_TIE_MASK;
		NVIC_EnableIRQ(PIT_IRQn);
		NVIC_SetPriority(PIT_IRQn, config.interrupt_priority);
	}
	if (config.start)
	{
		Start();
	}
}

void Pit::ChangePeriod(uint32_t us)
{
	ChangePeriodCount(1.0 * pit_clock * us / (1000000));
}

extern "C"
{

	void PIT_IRQHandler()
	{
		if (PIT->CHANNEL[0].TFLG)
		{
			PIT->CHANNEL[0].TFLG = 1;
			pit_ptr[0]->GetListener()(pit_ptr[0]);
		}
		else if (PIT->CHANNEL[1].TFLG)
		{
			PIT->CHANNEL[1].TFLG = 1;
			pit_ptr[1]->GetListener()(pit_ptr[1]);
		}
		else if (PIT->CHANNEL[2].TFLG)
		{
			PIT->CHANNEL[2].TFLG = 1;
			pit_ptr[2]->GetListener()(pit_ptr[2]);
		}
		else if (PIT->CHANNEL[3].TFLG)
		{
			PIT->CHANNEL[3].TFLG = 1;
			pit_ptr[3]->GetListener()(pit_ptr[3]);
		}
	}
}

} // namespace Driver
