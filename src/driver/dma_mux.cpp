/*
 * dma_mux.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: LeeChunHei
 */

#include "driver/dma_mux.h"
#include "system/clock.h"

namespace Driver
{

using namespace System;
static const clock_ip_name_t s_dmamuxClockName[] = DMAMUX_CLOCKS;

DMAMux::DMAMux()
{
	dma_mux_base = (DMAMUX_Type *)0x400EC000u;
	CLOCK_EnableClock(s_dmamuxClockName[0]);
}

} // namespace Driver
