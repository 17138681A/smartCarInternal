/*
 * dma.cpp
 *
 *  Created on: Aug 31, 2018
 *      Author: LeeChunHei
 */

#include "driver/dma.h"
#include "system/clock.h"
#include <vector>

namespace Driver
{

DMA *dma_ptr;
DMA::DMA_Listener dma_listener[32];

using namespace System;
static const clock_ip_name_t s_edmaClockName[] = EDMA_CLOCKS;

DMA::DMA(const Config &config)
{
	CLOCK_EnableClock(s_edmaClockName[0]);

	dma_base = (DMA_Type *)0x400E8000u;
	/* Configure EDMA peripheral according to the configuration structure. */
	uint32_t tmpreg;
	tmpreg = dma_base->CR;
	tmpreg &= ~(DMA_CR_ERCA_MASK | DMA_CR_HOE_MASK | DMA_CR_CLM_MASK | DMA_CR_EDBG_MASK);
	tmpreg |= (DMA_CR_ERCA(config.enable_round_robin_arbitration) | DMA_CR_HOE(config.enable_halt_on_error) | DMA_CR_CLM(config.enable_continuous_link_mode) | DMA_CR_EDBG(config.enable_debug_mode) | DMA_CR_EMLM(true));
	dma_base->CR = tmpreg;
}

void DMA::ConfigChannel(const ChannelConfig &config)
{

	if (config.listener)
	{
		dma_ptr = this;
		dma_listener[config.channel] = config.listener;
		NVIC_EnableIRQ((IRQn_Type)(config.channel % 16));
		NVIC_SetPriority(IRQn_Type(config.channel % 16), config.interrupt_priority);
	}

	/*
	 Reset TCD registers to zero. Unlike the EDMA_TcdReset(DREQ will be set),
	 CSR will be 0. Because in order to suit EDMA busy check mechanism in
	 EDMA_SubmitTransfer, CSR must be set 0.
	 */
	dma_base->TCD[config.channel].SADDR = 0;
	dma_base->TCD[config.channel].SOFF = 0;
	dma_base->TCD[config.channel].ATTR = 0;
	dma_base->TCD[config.channel].NBYTES_MLNO = 0;
	dma_base->TCD[config.channel].SLAST = 0;
	dma_base->TCD[config.channel].DADDR = 0;
	dma_base->TCD[config.channel].DOFF = 0;
	dma_base->TCD[config.channel].CITER_ELINKNO = 0;
	dma_base->TCD[config.channel].DLAST_SGA = 0;
	dma_base->TCD[config.channel].CSR = 0;
	dma_base->TCD[config.channel].BITER_ELINKNO = 0;
}

bool DMA::ConfigTransfer(const TransferConfig &config)
{

	/*
	 Check if EDMA is busy: if the given channel started transfer, CSR will be not zero. Because
	 if it is the last transfer, DREQ will be set. If not, ESG will be set. So in order to suit
	 this check mechanism, EDMA_CreatHandle will clear CSR register.
	 */
	if ((dma_base->TCD[config.channel].CSR) && ((dma_base->TCD[config.channel].CSR & DMA_CSR_DONE_MASK) == 0))
	{
		return false;
	}
	else
	{
		/* source address */
		dma_base->TCD[config.channel].SADDR = config.src_addr;
		/* destination address */
		dma_base->TCD[config.channel].DADDR = config.dest_addr;
		/* Source data and destination data transfer size */
		dma_base->TCD[config.channel].ATTR = DMA_ATTR_SSIZE((uint32_t)config.src_transfer_size) | DMA_ATTR_DSIZE((uint32_t)config.dest_transfer_size);
		/* Source address signed offset */
		dma_base->TCD[config.channel].SOFF = config.src_offset;
		/* Destination address signed offset */
		dma_base->TCD[config.channel].DOFF = config.dest_offset;
		/* Minor byte transfer count */
		dma_base->TCD[config.channel].NBYTES_MLNO = config.minor_loop_bytes;
		/* Current major iteration count */
		dma_base->TCD[config.channel].CITER_ELINKNO = config.major_loop_counts;
		/* Starting major iteration count */
		dma_base->TCD[config.channel].BITER_ELINKNO = config.major_loop_counts;
		/* Enable auto disable request feature */
		dma_base->TCD[config.channel].CSR |= DMA_CSR_DREQ_MASK;
		/* Enable major interrupt */
		dma_base->TCD[config.channel].CSR |= DMA_CSR_INTMAJOR_MASK;

		return true;
	}
}

void DMA::StartTransfer(const uint32_t channel)
{
	dma_base->SERQ = DMA_SERQ_SERQ(channel);
}

extern "C"
{
	void DMA0_DMA16_IRQHandler()
	{
		if (DMA0->INT & 1 << 0)
		{
			dma_listener[0](dma_ptr);
			DMA0->CINT = 0;
		}
		if (DMA0->INT & 1 << 16)
		{
			dma_listener[16](dma_ptr);
			DMA0->CINT = 16;
		}
	}

	void DMA1_DMA17_IRQHandler()
	{
		if (DMA0->INT & 1 << 1)
		{
			dma_listener[1](dma_ptr);
			DMA0->CINT = 1;
		}
		if (DMA0->INT & 1 << 17)
		{
			dma_listener[17](dma_ptr);
			DMA0->CINT = 17;
		}
	}

	void DMA2_DMA18_IRQHandler()
	{
		if (DMA0->INT & 1 << 2)
		{
			dma_listener[2](dma_ptr);
			DMA0->CINT = 2;
		}
		if (DMA0->INT & 1 << 18)
		{
			dma_listener[18](dma_ptr);
			DMA0->CINT = 18;
		}
	}

	void DMA3_DMA19_IRQHandler()
	{
		if (DMA0->INT & 1 << 3)
		{
			dma_listener[3](dma_ptr);
			DMA0->CINT = 3;
		}
		if (DMA0->INT & 1 << 19)
		{
			dma_listener[19](dma_ptr);
			DMA0->CINT = 19;
		}
	}

	void DMA4_DMA20_IRQHandler()
	{
		if (DMA0->INT & 1 << 4)
		{
			dma_listener[4](dma_ptr);
			DMA0->CINT = 4;
		}
		if (DMA0->INT & 1 << 20)
		{
			dma_listener[20](dma_ptr);
			DMA0->CINT = 20;
		}
	}

	void DMA5_DMA21_IRQHandler()
	{
		if (DMA0->INT & 1 << 5)
		{
			dma_listener[5](dma_ptr);
			DMA0->CINT = 5;
		}
		if (DMA0->INT & 1 << 21)
		{
			dma_listener[21](dma_ptr);
			DMA0->CINT = 21;
		}
	}

	void DMA6_DMA22_IRQHandler()
	{
		if (DMA0->INT & 1 << 6)
		{
			dma_listener[6](dma_ptr);
			DMA0->CINT = 6;
		}
		if (DMA0->INT & 1 << 22)
		{
			dma_listener[22](dma_ptr);
			DMA0->CINT = 22;
		}
	}

	void DMA7_DMA23_IRQHandler()
	{
		if (DMA0->INT & 1 << 7)
		{
			dma_listener[7](dma_ptr);
			DMA0->CINT = 7;
		}
		if (DMA0->INT & 1 << 23)
		{
			dma_listener[23](dma_ptr);
			DMA0->CINT = 23;
		}
	}

	void DMA8_DMA24_IRQHandler()
	{
		if (DMA0->INT & 1 << 8)
		{
			dma_listener[8](dma_ptr);
			DMA0->CINT = 8;
		}
		if (DMA0->INT & 1 << 24)
		{
			dma_listener[24](dma_ptr);
			DMA0->CINT = 24;
		}
	}

	void DMA9_DMA25_IRQHandler()
	{
		if (DMA0->INT & 1 << 9)
		{
			dma_listener[9](dma_ptr);
			DMA0->CINT = 9;
		}
		if (DMA0->INT & 1 << 25)
		{
			dma_listener[25](dma_ptr);
			DMA0->CINT = 25;
		}
	}

	void DMA10_DMA26_IRQHandler()
	{
		if (DMA0->INT & 1 << 10)
		{
			dma_listener[10](dma_ptr);
			DMA0->CINT = 10;
		}
		if (DMA0->INT & 1 << 26)
		{
			dma_listener[26](dma_ptr);
			DMA0->CINT = 26;
		}
	}

	void DMA11_DMA27_IRQHandler()
	{
		if (DMA0->INT & 1 << 11)
		{
			dma_listener[11](dma_ptr);
			DMA0->CINT = 11;
		}
		if (DMA0->INT & 1 << 27)
		{
			dma_listener[27](dma_ptr);
			DMA0->CINT = 27;
		}
	}

	void DMA12_DMA28_IRQHandler()
	{
		if (DMA0->INT & 1 << 12)
		{
			dma_listener[12](dma_ptr);
			DMA0->CINT = 12;
		}
		if (DMA0->INT & 1 << 28)
		{
			dma_listener[28](dma_ptr);
			DMA0->CINT = 28;
		}
	}

	void DMA13_DMA29_IRQHandler()
	{
		if (DMA0->INT & 1 << 13)
		{
			dma_listener[13](dma_ptr);
			DMA0->CINT = 13;
		}
		if (DMA0->INT & 1 << 29)
		{
			dma_listener[29](dma_ptr);
			DMA0->CINT = 29;
		}
	}

	void DMA14_DMA30_IRQHandler()
	{
		if (DMA0->INT & 1 << 14)
		{
			dma_listener[14](dma_ptr);
			DMA0->CINT = 14;
		}
		if (DMA0->INT & 1 << 30)
		{
			dma_listener[30](dma_ptr);
			DMA0->CINT = 30;
		}
	}

	void DMA15_DMA31_IRQHandler()
	{
		if (DMA0->INT & 1 << 15)
		{
			dma_listener[15](dma_ptr);
			DMA0->CINT = 15;
		}
		if (DMA0->INT & 1 << 31)
		{
			dma_listener[31](dma_ptr);
			DMA0->CINT = 31;
		}
	}
}

} // namespace Driver
