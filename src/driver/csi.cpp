/*
 * csi.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: LeeChunHei
 */

#include "driver/csi.h"
#include "system/clock.h"
#include <assert.h>

namespace Driver
{

Csi *csi_ptr;

using namespace System;
const clock_ip_name_t csi_clocks[] = CSI_CLOCKS;

Csi::Csi(const Config &config)
{
	uint32_t img_width_bytes = config.width * config.bytes_per_pixel;

	/* The image width and frame buffer pitch should be multiple of 8-bytes. */
	assert(!(img_width_bytes & 0x07) && !((uint32_t)config.line_pitch_bytes & 0x07));

	uint8_t module, prev_module;
	Pinout::Config pin_config;

	pin_config.pin = config.pin_list.data2;
	if (Pinout::GetCSIData2PinConfig(pin_config, prev_module))
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data3;
	if (Pinout::GetCSIData3PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data4;
	if (Pinout::GetCSIData4PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data5;
	if (Pinout::GetCSIData5PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data6;
	if (Pinout::GetCSIData6PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data7;
	if (Pinout::GetCSIData7PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data8;
	if (Pinout::GetCSIData8PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	pin_config.pin = config.pin_list.data9;
	if (Pinout::GetCSIData9PinConfig(pin_config, module) && module == prev_module)
	{
		Pinout::InitPin(pin_config);
	}
	else
	{
		return;
	}
	if (config.pin_list.data0 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data0;
		if (Pinout::GetCSIData0PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data1 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data1;
		if (Pinout::GetCSIData1PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data10 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data10;
		if (Pinout::GetCSIData10PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data11 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data11;
		if (Pinout::GetCSIData11PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data12 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data12;
		if (Pinout::GetCSIData12PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data13 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data13;
		if (Pinout::GetCSIData13PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data14 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data14;
		if (Pinout::GetCSIData14PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data15 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data15;
		if (Pinout::GetCSIData15PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data16 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data16;
		if (Pinout::GetCSIData16PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data17 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data17;
		if (Pinout::GetCSIData17PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data18 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data18;
		if (Pinout::GetCSIData18PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data19 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data19;
		if (Pinout::GetCSIData19PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data20 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data20;
		if (Pinout::GetCSIData20PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data21 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data21;
		if (Pinout::GetCSIData21PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data22 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data22;
		if (Pinout::GetCSIData22PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.data23 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data23;
		if (Pinout::GetCSIData23PinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.field != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.field;
		if (Pinout::GetCSIFieldPinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.hsync != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.hsync;
		if (Pinout::GetCSIHsyncPinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.mclk != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.mclk;
		if (Pinout::GetCSIMclkPinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.pclk != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.pclk;
		if (Pinout::GetCSIPclkPinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}
	if (config.pin_list.vsync != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.vsync;
		if (Pinout::GetCSIVsyncPinConfig(pin_config, module) && module == prev_module)
		{
			Pinout::InitPin(pin_config);
		}
		else
		{
			return;
		}
	}

	if (config.listener)
	{
		csi_ptr = this;
		listener = config.listener;
		NVIC_EnableIRQ(CSI_IRQn);
		NVIC_SetPriority(CSI_IRQn, config.interrupt_priority);
		csi_base->CSICR1 |= (0 | CSI_CSICR1_FB2_DMA_DONE_INTEN_MASK | CSI_CSICR1_FB1_DMA_DONE_INTEN_MASK);
	}

	CLOCK_EnableClock(csi_clocks[module]);

	csi_base = (CSI_Type *)(0x402BC000u);

	Reset();

	/* Configure CSICR1. CSICR1 has been reset to the default value, so could write it directly. */
	uint32_t reg = config.is_10_bit_data | ((uint32_t)config.work_mode) | ((uint32_t)config.hsync_active_high << 11u) | ((uint32_t)config.data_latch_on_rising_edge << 1u) | ((uint32_t)config.vsync_active_low << 17u) | CSI_CSICR1_FCC_MASK;

	if (config.use_ext_vsync)
	{
		reg |= CSI_CSICR1_EXT_VSYNC_MASK;
	}

	csi_base->CSICR1 = reg;

	/*
	 * Generally, CSIIMAG_PARA[IMAGE_WIDTH] indicates how many data bus cycles per line.
	 * One special case is when receiving 24-bit pixels through 8-bit data bus, and
	 * CSICR3[ZERO_PACK_EN] is enabled, in this case, the CSIIMAG_PARA[IMAGE_WIDTH]
	 * should be set to the pixel number per line.
	 *
	 * Currently the CSI driver only support 8-bit data bus, so generally the
	 * CSIIMAG_PARA[IMAGE_WIDTH] is bytes number per line. When the CSICR3[ZERO_PACK_EN]
	 * is enabled, CSIIMAG_PARA[IMAGE_WIDTH] is pixel number per line.
	 *
	 * NOTE: The CSIIMAG_PARA[IMAGE_WIDTH] setting code should be updated if the
	 * driver is upgraded to support other data bus width.
	 */
	if (config.bytes_per_pixel == 4u)
	{
		/* Enable zero pack. */
		csi_base->CSICR3 |= CSI_CSICR3_ZERO_PACK_EN_MASK;
		/* Image parameter. */
		csi_base->CSIIMAG_PARA = ((uint32_t)(config.width) << CSI_CSIIMAG_PARA_IMAGE_WIDTH_SHIFT) | ((uint32_t)(config.height) << CSI_CSIIMAG_PARA_IMAGE_HEIGHT_SHIFT);
	}
	else
	{
		/* Image parameter. */
		csi_base->CSIIMAG_PARA = ((uint32_t)(img_width_bytes) << CSI_CSIIMAG_PARA_IMAGE_WIDTH_SHIFT) | ((uint32_t)(config.height) << CSI_CSIIMAG_PARA_IMAGE_HEIGHT_SHIFT);
	}

	/* The CSI frame buffer bus is 8-byte width. */
	csi_base->CSIFBUF_PARA = (uint32_t)((config.line_pitch_bytes - img_width_bytes) / 8U) << CSI_CSIFBUF_PARA_FBUF_STRIDE_SHIFT;

	/* Enable auto ECC. */
	csi_base->CSICR3 |= CSI_CSICR3_ECC_AUTO_EN_MASK;

	/*
	 * For better performance.
	 * The DMA burst size could be set to 16 * 8 byte, 8 * 8 byte, or 4 * 8 byte,
	 * choose the best burst size based on bytes per line.
	 */
	if (!(img_width_bytes % (8 * 16)))
	{
		csi_base->CSICR2 = CSI_CSICR2_DMA_BURST_TYPE_RFF(3U);
		csi_base->CSICR3 = (csi_base->CSICR3 & ~CSI_CSICR3_RxFF_LEVEL_MASK) | ((2U << CSI_CSICR3_RxFF_LEVEL_SHIFT));
	}
	else if (!(img_width_bytes % (8 * 8)))
	{
		csi_base->CSICR2 = CSI_CSICR2_DMA_BURST_TYPE_RFF(2U);
		csi_base->CSICR3 = (csi_base->CSICR3 & ~CSI_CSICR3_RxFF_LEVEL_MASK) | ((1U << CSI_CSICR3_RxFF_LEVEL_SHIFT));
	}
	else
	{
		csi_base->CSICR2 = CSI_CSICR2_DMA_BURST_TYPE_RFF(1U);
		csi_base->CSICR3 = (csi_base->CSICR3 & ~CSI_CSICR3_RxFF_LEVEL_MASK) | ((0U << CSI_CSICR3_RxFF_LEVEL_SHIFT));
	}

	ReflashFIFODma(1u);
}

void Csi::SetListener(Csi_Listener listener, uint8_t interrupt_priority)
{
	csi_ptr = this;
	this->listener = listener;
	NVIC_EnableIRQ(CSI_IRQn);
	NVIC_SetPriority(CSI_IRQn, interrupt_priority);
	csi_base->CSICR1 |= (0 | CSI_CSICR1_FB2_DMA_DONE_INTEN_MASK | CSI_CSICR1_FB1_DMA_DONE_INTEN_MASK);
}

void Csi::Reset()
{
	/* Disable transfer first. */
	Stop();

	/* Disable DMA request. */
	csi_base->CSICR3 = 0u;

	/* Reset the fame count. */
	csi_base->CSICR3 |= CSI_CSICR3_FRMCNT_RST_MASK;
	while (csi_base->CSICR3 & CSI_CSICR3_FRMCNT_RST_MASK)
	{
	}

	/* Clear the RX FIFO and static FIFO. */
	ClearFIFO(3u);

	/* Reflash DMA. */
	ReflashFIFODma(3u);

	/* Clear the status. */
	uint32_t reg = csi_base->CSISR;
	csi_base->CSISR = reg;

	/* Set the control registers to default value. */
	csi_base->CSICR1 = CSI_CSICR1_HSYNC_POL_MASK | CSI_CSICR1_EXT_VSYNC_MASK;
	csi_base->CSICR2 = 0U;
	csi_base->CSICR3 = 0U;
	csi_base->CSICR18 = CSI_CSICR18_AHB_HPROT(0x0DU);
	csi_base->CSIFBUF_PARA = 0U;
	csi_base->CSIIMAG_PARA = 0U;
}

void Csi::Start()
{
	/* Clear the status. */
	uint32_t reg = csi_base->CSISR;
	csi_base->CSISR = reg;
	csi_base->CSICR18 = (csi_base->CSICR18 & ~CSI_CSICR18_MASK_OPTION_MASK) | CSI_CSICR18_MASK_OPTION(2);
	ReflashFIFODma(1u);
	EnableFIFODmaRequest(3u);
	csi_base->CSICR18 |= CSI_CSICR18_CSI_ENABLE_MASK;
}

void Csi::Stop()
{
	csi_base->CSICR18 &= ~CSI_CSICR18_CSI_ENABLE_MASK;
	EnableFIFODmaRequest(2u); //Disable rx fifo dma request
}

void Csi::ClearFIFO(uint8_t fifo)
{
	/* The FIFO could only be cleared when CSICR1[FCC] = 0, so first clear the FCC. */
	uint32_t cr1 = csi_base->CSICR1;
	csi_base->CSICR1 = (cr1 & ~CSI_CSICR1_FCC_MASK);

	uint32_t mask = 0;
	if (fifo & 1u)
	{
		mask |= CSI_CSICR1_CLR_RXFIFO_MASK;
	}

	if (fifo & 2u)
	{
		mask |= CSI_CSICR1_CLR_STATFIFO_MASK;
	}

	csi_base->CSICR1 = (cr1 & ~CSI_CSICR1_FCC_MASK) | mask;

	/* Wait clear completed. */
	while (csi_base->CSICR1 & mask)
	{
	}

	/* Recover the FCC. */
	csi_base->CSICR1 = cr1;
}

void Csi::ReflashFIFODma(uint8_t fifo)
{
	uint32_t cr3 = 0;

	if (fifo & 1u)
	{
		cr3 |= CSI_CSICR3_DMA_REFLASH_RFF_MASK;
	}

	if (fifo & 2u)
	{
		cr3 |= CSI_CSICR3_DMA_REFLASH_SFF_MASK;
	}

	csi_base->CSICR3 |= cr3;

	/* Wait clear completed. */
	while (csi_base->CSICR3 & cr3)
	{
	}
}

void Csi::EnableFIFODmaRequest(uint8_t fifo_enable)
{
	uint32_t cr3 = 0u;

	if (fifo_enable & 2u)
	{
		cr3 |= CSI_CSICR3_DMA_REQ_EN_RFF_MASK;
	}

	if (fifo_enable & 4u)
	{
		cr3 |= CSI_CSICR3_DMA_REQ_EN_SFF_MASK;
	}

	if (fifo_enable & 1u)
	{
		csi_base->CSICR3 |= cr3;
	}
	else
	{
		csi_base->CSICR3 &= ~cr3;
	}
}

void Csi::ConfigTransferBuffer(uint8_t index, uint32_t addr)
{
	if (index)
	{
		csi_base->CSIDMASA_FB2 = addr;
	}
	else
	{
		csi_base->CSIDMASA_FB1 = addr;
	}
}

uint32_t Csi::GetTransferBuffer(uint8_t index)
{
	if (index)
	{
		return csi_base->CSIDMASA_FB2;
	}
	else
	{
		return csi_base->CSIDMASA_FB1;
	}
}

extern "C"
{
	void CSI_IRQHandler()
	{
		csi_ptr->GetListener()(csi_ptr);
		CSI->CSISR = 0xFFFFFFFF;
	}
}

} // namespace Driver
