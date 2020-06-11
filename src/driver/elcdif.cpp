/*
 * elcdif.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: LeeChunHei
 */

#include "driver/elcdif.h"
#include "system/clock.h"
#include "system/system.h"

namespace Driver
{

eLCDIF *elcdif_ptr;

using namespace System;

const clock_ip_name_t s_elcdifApbClocks[] = LCDIF_CLOCKS;
const clock_ip_name_t s_elcdifPixClocks[] = LCDIF_PERIPH_CLOCKS;

const uint32_t pixel_format_reg[5][2] = {
	/* kELCDIF_PixelFormatRAW8 */
	{/* Register CTRL. */
	 LCDIF_CTRL_WORD_LENGTH(1U),
	 /* Register CTRL1. */
	 LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x0FU)},
	/* kELCDIF_PixelFormatRGB565 */
	{/* Register CTRL. */
	 LCDIF_CTRL_WORD_LENGTH(0U),
	 /* Register CTRL1. */
	 LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x0FU)},
	/* kELCDIF_PixelFormatRGB666 */
	{/* Register CTRL. */
	 LCDIF_CTRL_WORD_LENGTH(3U) | LCDIF_CTRL_DATA_FORMAT_24_BIT(1U),
	 /* Register CTRL1. */
	 LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x07U)},
	/* kELCDIF_PixelFormatXRGB8888 */
	{/* Register CTRL. 24-bit. */
	 LCDIF_CTRL_WORD_LENGTH(3U),
	 /* Register CTRL1. */
	 LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x07U)},
	/* kELCDIF_PixelFormatRGB888 */
	{/* Register CTRL. 24-bit. */
	 LCDIF_CTRL_WORD_LENGTH(3U),
	 /* Register CTRL1. */
	 LCDIF_CTRL1_BYTE_PACKING_FORMAT(0x0FU)}};

eLCDIF::eLCDIF(const Config &config)
{

	uint8_t module, prev_module;
	Pinout::Config pin_config;

	pin_config.pin = config.pin_list.pclk;
	assert(Pinout::GeteLCDIFPclkPinConfig(pin_config, prev_module));
	Pinout::InitPin(pin_config);
	pin_config.pin = config.pin_list.hsync;
	assert(Pinout::GeteLCDIFHsyncPinConfig(pin_config, module) && module == prev_module);
	Pinout::InitPin(pin_config);
	pin_config.pin = config.pin_list.vsync;
	assert(Pinout::GeteLCDIFVsyncPinConfig(pin_config, module));
	Pinout::InitPin(pin_config);
	pin_config.pin = config.pin_list.enable;
	assert(Pinout::GeteLCDIFEnablePinConfig(pin_config, module));
	Pinout::InitPin(pin_config);

	if (config.pin_list.data0 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data0;
		assert(Pinout::GeteLCDIFData0PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data1 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data1;
		assert(Pinout::GeteLCDIFData1PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data2 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data2;
		assert(Pinout::GeteLCDIFData2PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data3 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data3;
		assert(Pinout::GeteLCDIFData3PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data4 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data4;
		assert(Pinout::GeteLCDIFData4PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data5 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data5;
		assert(Pinout::GeteLCDIFData5PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data6 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data6;
		assert(Pinout::GeteLCDIFData6PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data7 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data7;
		assert(Pinout::GeteLCDIFData7PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data8 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data8;
		assert(Pinout::GeteLCDIFData8PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data9 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data9;
		assert(Pinout::GeteLCDIFData9PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data10 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data10;
		assert(Pinout::GeteLCDIFData10PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data11 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data11;
		assert(Pinout::GeteLCDIFData11PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data12 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data12;
		assert(Pinout::GeteLCDIFData12PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data13 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data13;
		assert(Pinout::GeteLCDIFData13PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data14 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data14;
		assert(Pinout::GeteLCDIFData14PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data15 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data15;
		assert(Pinout::GeteLCDIFData15PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data16 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data16;
		assert(Pinout::GeteLCDIFData16PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data17 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data17;
		assert(Pinout::GeteLCDIFData17PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data18 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data18;
		assert(Pinout::GeteLCDIFData18PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data19 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data19;
		assert(Pinout::GeteLCDIFData19PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data20 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data20;
		assert(Pinout::GeteLCDIFData20PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data21 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data21;
		assert(Pinout::GeteLCDIFData21PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data22 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data22;
		assert(Pinout::GeteLCDIFData22PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}
	if (config.pin_list.data23 != Pinout::Name::kDisable)
	{
		pin_config.pin = config.pin_list.data23;
		assert(Pinout::GeteLCDIFData23PinConfig(pin_config, module));
		Pinout::InitPin(pin_config);
	}

	uint32_t desire_pclk = (config.width + config.hsync_pulse_width + config.horizontal_front_porch + config.horizontal_back_porch) * (config.height + config.vsync_pulse_width + config.vertical_front_porch + config.vertical_back_porch) * config.frame_rate;
	uint32_t best_difference = 4294967295;
	uint32_t best_loopDivider, best_preDiv, best_div, best_postDivider;
	uint32_t postDividerList[5] = {1, 2, 4, 8, 16};
	for (uint32_t loopDivider = 27; loopDivider < 55; ++loopDivider)
	{
		for (uint32_t postDivider = 0; postDivider < 5; ++postDivider)
		{
			for (uint32_t preDiv = 1; preDiv < 9; ++preDiv)
			{
				for (uint32_t div = 1; div < 9; ++div)
				{
					uint32_t clock = ((24000000 * (loopDivider) / postDividerList[postDivider]) / preDiv) / div;
					uint32_t difference;
					if (clock > desire_pclk)
					{
						difference = clock - desire_pclk;
					}
					else
					{
						difference = desire_pclk - clock;
					}
					if (difference < best_difference)
					{
						best_difference = difference;
						best_loopDivider = loopDivider;
						best_preDiv = preDiv;
						best_div = div;
						best_postDivider = postDivider;
					}
				}
			}
		}
	}

	clock_video_pll_config_t pll_config = {.loopDivider = (uint8_t)best_loopDivider, .postDivider = (uint8_t)postDividerList[best_postDivider], .numerator = 0, .denominator = 0, .src = (uint8_t)0};

	CLOCK_InitVideoPll(&pll_config);
	CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);
	CLOCK_SetDiv(kCLOCK_LcdifPreDiv, best_preDiv);
	CLOCK_SetDiv(kCLOCK_LcdifDiv, best_div);

	/* Enable the clock. */
	CLOCK_EnableClock(s_elcdifApbClocks[module]);
	CLOCK_EnableClock(s_elcdifPixClocks[module]);

	elcdif_base = (LCDIF_Type *)(0x402B8000u);

	/* Reset. */
	Reset();

	elcdif_base->CTRL = pixel_format_reg[(uint32_t)config.pixel_format][0] | (uint32_t)(config.data_bus) | LCDIF_CTRL_DOTCLK_MODE_MASK | /* RGB mode. */
						LCDIF_CTRL_BYPASS_COUNT_MASK |																					 /* Keep RUN bit set. */
						LCDIF_CTRL_MASTER_MASK;

	elcdif_base->CTRL1 = pixel_format_reg[(uint32_t)config.pixel_format][1];

	elcdif_base->TRANSFER_COUNT = ((uint32_t)config.height << LCDIF_TRANSFER_COUNT_V_COUNT_SHIFT) | ((uint32_t)config.width << LCDIF_TRANSFER_COUNT_H_COUNT_SHIFT);

	elcdif_base->VDCTRL0 = LCDIF_VDCTRL0_ENABLE_PRESENT_MASK |		   /* Data enable signal. */
						   LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT_MASK |	  /* VSYNC period in the unit of display clock. */
						   LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT_MASK | /* VSYNC pulse width in the unit of display clock. */
						   (uint32_t)(config.vsync_active_high << LCDIF_VDCTRL0_VSYNC_POL_SHIFT | config.hsync_active_high << LCDIF_VDCTRL0_HSYNC_POL_SHIFT | config.data_enable_active_high << LCDIF_VDCTRL0_ENABLE_POL_SHIFT | config.drive_data_on_rising_clk_edge << LCDIF_VDCTRL0_DOTCLK_POL_SHIFT) | (uint32_t)config.vsync_pulse_width;

	elcdif_base->VDCTRL1 = config.vsync_pulse_width + config.height + config.vertical_front_porch + config.vertical_back_porch;
	elcdif_base->VDCTRL2 = ((uint32_t)config.hsync_pulse_width << LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_SHIFT) | ((uint32_t)(config.horizontal_front_porch + config.horizontal_back_porch + config.width + config.hsync_pulse_width)) << LCDIF_VDCTRL2_HSYNC_PERIOD_SHIFT;

	elcdif_base->VDCTRL3 = (((uint32_t)config.horizontal_back_porch + config.hsync_pulse_width) << LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_SHIFT) | (((uint32_t)config.vertical_back_porch + config.vsync_pulse_width) << LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_SHIFT);

	elcdif_base->VDCTRL4 = LCDIF_VDCTRL4_SYNC_SIGNALS_ON_MASK | ((uint32_t)config.width << LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_SHIFT);

	elcdif_base->CUR_BUF = config.buffer_addr;
	elcdif_base->NEXT_BUF = config.buffer_addr;

	if (config.listener)
	{
		elcdif_ptr = this;
		listener = config.listener;
		NVIC_EnableIRQ(LCDIF_IRQn);
		NVIC_SetPriority(LCDIF_IRQn, config.interrupt_priority);
		elcdif_base->CTRL1_SET = (LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN_MASK & (LCDIF_CTRL1_BM_ERROR_IRQ_EN_MASK | LCDIF_CTRL1_OVERFLOW_IRQ_EN_MASK | LCDIF_CTRL1_UNDERFLOW_IRQ_EN_MASK | LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN_MASK | LCDIF_CTRL1_VSYNC_EDGE_IRQ_EN_MASK));
	}
}

void eLCDIF::Reset()
{
	/* Disable the clock gate. */
	elcdif_base->CTRL_CLR = LCDIF_CTRL_CLKGATE_MASK;
	/* Confirm the clock gate is disabled. */
	while (elcdif_base->CTRL & LCDIF_CTRL_CLKGATE_MASK)
	{
	}

	/* Reset the block. */
	elcdif_base->CTRL_SET = LCDIF_CTRL_SFTRST_MASK;
	/* Confirm the reset bit is set. */
	while (!(elcdif_base->CTRL & LCDIF_CTRL_SFTRST_MASK))
	{
	}

	volatile uint32_t i = 0x100;
	/* Delay for the reset. */
	while (i--)
	{
	}

	/* Bring the module out of reset. */
	elcdif_base->CTRL_CLR = LCDIF_CTRL_SFTRST_MASK;
	/* Disable the clock gate. */
	elcdif_base->CTRL_CLR = LCDIF_CTRL_CLKGATE_MASK;
}

void eLCDIF::SetListener(eLCDIF_Listener listener, uint8_t interrupt_priority)
{
	elcdif_ptr = this;
	this->listener = listener;
	NVIC_EnableIRQ(LCDIF_IRQn);
	NVIC_SetPriority(LCDIF_IRQn, interrupt_priority);
	elcdif_base->CTRL1_SET = (LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN_MASK & (LCDIF_CTRL1_BM_ERROR_IRQ_EN_MASK | LCDIF_CTRL1_OVERFLOW_IRQ_EN_MASK | LCDIF_CTRL1_UNDERFLOW_IRQ_EN_MASK | LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN_MASK | LCDIF_CTRL1_VSYNC_EDGE_IRQ_EN_MASK));
}

extern "C"
{
	void LCDIF_IRQHandler()
	{
		uint32_t status = LCDIF->CTRL1 & (LCDIF_CTRL1_BM_ERROR_IRQ_MASK | LCDIF_CTRL1_OVERFLOW_IRQ_MASK | LCDIF_CTRL1_UNDERFLOW_IRQ_MASK | LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_MASK | LCDIF_CTRL1_VSYNC_EDGE_IRQ_MASK);
		LCDIF->CTRL1_CLR = status;
		elcdif_ptr->GetListener()(elcdif_ptr);
	}
}

} // namespace Driver
