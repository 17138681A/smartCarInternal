/*
 * dma_mux.h
 *
 *  Created on: Nov 4, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_DMA_MUX_H_
#define INC_DRIVER_DMA_MUX_H_

#include "system/cmsis/access_layer/access_layer.h"

namespace Driver
{

/*!
 * @ingroup dma
 * @{
 */
/**
 * @brief Direct Memory Access Multiplexer
 */
class DMAMux
{
  public:
	enum struct RequestSource
	{
		kFlexIO1Request0Request1 = 0 | 0x100U,					   /**< FlexIO1 */
		kFlexIO2Request0Request1 = 1 | 0x100U,					   /**< FlexIO2 */
		kLPUART1Tx = 2 | 0x100U,								   /**< LPUART1 Transmit */
		kLPUART1Rx = 3 | 0x100U,								   /**< LPUART1 Receive */
		kLPUART3Tx = 4 | 0x100U,								   /**< LPUART3 Transmit */
		kLPUART3Rx = 5 | 0x100U,								   /**< LPUART3 Receive */
		kLPUART5Tx = 6 | 0x100U,								   /**< LPUART5 Transmit */
		kLPUART5Rx = 7 | 0x100U,								   /**< LPUART5 Receive */
		kLPUART7Tx = 8 | 0x100U,								   /**< LPUART7 Transmit */
		kLPUART7Rx = 9 | 0x100U,								   /**< LPUART7 Receive */
		kCSI = 12 | 0x100U,										   /**< CSI */
		kLPSPI1Rx = 13 | 0x100U,								   /**< LPSPI1 Receive */
		kLPSPI1Tx = 14 | 0x100U,								   /**< LPSPI1 Transmit */
		kLPSPI3Rx = 15 | 0x100U,								   /**< LPSPI3 Receive */
		kLPSPI3Tx = 16 | 0x100U,								   /**< LPSPI3 Transmit */
		kLPI2C1 = 17 | 0x100U,									   /**< LPI2C1 */
		kLPI2C3 = 18 | 0x100U,									   /**< LPI2C3 */
		kSai1Rx = 19 | 0x100U,									   /**< Sai1 Receive */
		kSai1Tx = 20 | 0x100U,									   /**< Sai1 Transmit */
		kSai2Rx = 21 | 0x100U,									   /**< Sai2 Receive */
		kSai2Tx = 22 | 0x100U,									   /**< Sai2 Transmit */
		kADC_ETC = 23 | 0x100U,									   /**< ADC_ETC */
		kADC1 = 24 | 0x100U,									   /**< ADC1 */
		kACMP1 = 25 | 0x100U,									   /**< ACMP1 */
		kACMP2 = 26 | 0x100U,									   /**< ACMP2 */
		kFlexSPIRx = 28 | 0x100U,								   /**< FlexSPI Receive */
		kFlexSPITx = 29 | 0x100U,								   /**< FlexSPI Transmit */
		kXBAR1Request0 = 30 | 0x100U,							   /**< XBAR1 Request 0 */
		kXBAR1Request1 = 31 | 0x100U,							   /**< XBAR1 Request 1 */
		kFlexPWM1CaptureSub0 = 32 | 0x100U,						   /**< FlexPWM1 Capture sub-module0 */
		kFlexPWM1CaptureSub1 = 33 | 0x100U,						   /**< FlexPWM1 Capture sub-module1 */
		kFlexPWM1CaptureSub2 = 34 | 0x100U,						   /**< FlexPWM1 Capture sub-module2 */
		kFlexPWM1CaptureSub3 = 35 | 0x100U,						   /**< FlexPWM1 Capture sub-module3 */
		kFlexPWM1ValueSub0 = 36 | 0x100U,						   /**< FlexPWM1 Value sub-module0 */
		kFlexPWM1ValueSub1 = 37 | 0x100U,						   /**< FlexPWM1 Value sub-module1 */
		kFlexPWM1ValueSub2 = 38 | 0x100U,						   /**< FlexPWM1 Value sub-module2 */
		kFlexPWM1ValueSub3 = 39 | 0x100U,						   /**< FlexPWM1 Value sub-module3 */
		kFlexPWM3CaptureSub0 = 40 | 0x100U,						   /**< FlexPWM3 Capture sub-module0 */
		kFlexPWM3CaptureSub1 = 41 | 0x100U,						   /**< FlexPWM3 Capture sub-module1 */
		kFlexPWM3CaptureSub2 = 42 | 0x100U,						   /**< FlexPWM3 Capture sub-module2 */
		kFlexPWM3CaptureSub3 = 43 | 0x100U,						   /**< FlexPWM3 Capture sub-module3 */
		kFlexPWM3ValueSub0 = 44 | 0x100U,						   /**< FlexPWM3 Value sub-module0 */
		kFlexPWM3ValueSub1 = 45 | 0x100U,						   /**< FlexPWM3 Value sub-module1 */
		kFlexPWM3ValueSub2 = 46 | 0x100U,						   /**< FlexPWM3 Value sub-module2 */
		kFlexPWM3ValueSub3 = 47 | 0x100U,						   /**< FlexPWM3 Value sub-module3 */
		kQTIMER1CaptTimer0 = 48 | 0x100U,						   /**< QTIMER1 Capture timer 0 */
		kQTIMER1CaptTimer1 = 49 | 0x100U,						   /**< QTIMER1 Capture timer 1 */
		kQTIMER1CaptTimer2 = 50 | 0x100U,						   /**< QTIMER1 Capture timer 2 */
		kQTIMER1CaptTimer3 = 51 | 0x100U,						   /**< QTIMER1 Capture timer 3 */
		kQTIMER1Cmpld1Timer0Cmpld2Timer1 = 52 | 0x100U,			   /**< QTIMER1 cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER1Cmpld1Timer1Cmpld2Timer0 = 53 | 0x100U,			   /**< QTIMER1 cmpld1 in timer 1 or cmpld2 in timer 0 */
		kQTIMER1Cmpld1Timer2Cmpld2Timer3 = 54 | 0x100U,			   /**< QTIMER1 cmpld1 in timer 2 or cmpld2 in timer 3 */
		kQTIMER1Cmpld1Timer3Cmpld2Timer2 = 55 | 0x100U,			   /**< QTIMER1 cmpld1 in timer 3 or cmpld2 in timer 2 */
		kQTIMER3CaptTimer0Cmpld1Timer0Cmpld2Timer1 = 56 | 0x100U,  /**< QTIMER3 capture timer 0, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER3CaptTimer1Cmpld1Timer0Cmpld2Timer1 = 57 | 0x100U,  /**< QTIMER3 capture timer 1, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER3CaptTimer2Cmpld1Timer0Cmpld2Timer1 = 58 | 0x100U,  /**< QTIMER3 capture timer 2, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER3CaptTimer3Cmpld1Timer0Cmpld2Timer1 = 59 | 0x100U,  /**< QTIMER3 capture timer 3, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kFlexIO1Request2Request3 = 64 | 0x100U,					   /**< FlexIO1 */
		kFlexIO2Request2Request3 = 65 | 0x100U,					   /**< FlexIO2 */
		kLPUART2Tx = 66 | 0x100U,								   /**< LPUART2 Transmit */
		kLPUART2Rx = 67 | 0x100U,								   /**< LPUART2 Receive */
		kLPUART4Tx = 68 | 0x100U,								   /**< LPUART4 Transmit */
		kLPUART4Rx = 69 | 0x100U,								   /**< LPUART4 Receive */
		kLPUART6Tx = 70 | 0x100U,								   /**< LPUART6 Transmit */
		kLPUART6Rx = 71 | 0x100U,								   /**< LPUART6 Receive */
		kLPUART8Tx = 72 | 0x100U,								   /**< LPUART8 Transmit */
		kLPUART8Rx = 73 | 0x100U,								   /**< LPUART8 Receive */
		kPxp = 75 | 0x100U,										   /**< PXP */
		kLCDIF = 76 | 0x100U,									   /**< LCDIF */
		kLPSPI2Rx = 77 | 0x100U,								   /**< LPSPI2 Receive */
		kLPSPI2Tx = 78 | 0x100U,								   /**< LPSPI2 Transmit */
		kLPSPI4Rx = 79 | 0x100U,								   /**< LPSPI4 Receive */
		kLPSPI4Tx = 80 | 0x100U,								   /**< LPSPI4 Transmit */
		kLPI2C2 = 81 | 0x100U,									   /**< LPI2C2 */
		kLPI2C4 = 82 | 0x100U,									   /**< LPI2C4 */
		kSai3Rx = 83 | 0x100U,									   /**< Sai3 Receive */
		kSai3Tx = 84 | 0x100U,									   /**< Sai3 Transmit */
		kSpdifRx = 85 | 0x100U,									   /**< Spdif Receive */
		kSpdifTx = 86 | 0x100U,									   /**< Spdif Transmit */
		kADC2 = 88 | 0x100U,									   /**< ADC2 */
		kACMP3 = 89 | 0x100U,									   /**< ACMP3 */
		kACMP4 = 90 | 0x100U,									   /**< ACMP4 */
		kEnetTimer0 = 92 | 0x100U,								   /**< Enet Timer0 */
		kEnetTimer1 = 93 | 0x100U,								   /**< Enet Timer1 */
		kXBAR1Request2 = 94 | 0x100U,							   /**< XBAR1 Request 2 */
		kXBAR1Request3 = 95 | 0x100U,							   /**< XBAR1 Request 3 */
		kFlexPWM2CaptureSub0 = 96 | 0x100U,						   /**< FlexPWM2 Capture sub-module0 */
		kFlexPWM2CaptureSub1 = 97 | 0x100U,						   /**< FlexPWM2 Capture sub-module1 */
		kFlexPWM2CaptureSub2 = 98 | 0x100U,						   /**< FlexPWM2 Capture sub-module2 */
		kFlexPWM2CaptureSub3 = 99 | 0x100U,						   /**< FlexPWM2 Capture sub-module3 */
		kFlexPWM2ValueSub0 = 100 | 0x100U,						   /**< FlexPWM2 Value sub-module0 */
		kFlexPWM2ValueSub1 = 101 | 0x100U,						   /**< FlexPWM2 Value sub-module1 */
		kFlexPWM2ValueSub2 = 102 | 0x100U,						   /**< FlexPWM2 Value sub-module2 */
		kFlexPWM2ValueSub3 = 103 | 0x100U,						   /**< FlexPWM2 Value sub-module3 */
		kFlexPWM4CaptureSub0 = 104 | 0x100U,					   /**< FlexPWM4 Capture sub-module0 */
		kFlexPWM4CaptureSub1 = 105 | 0x100U,					   /**< FlexPWM4 Capture sub-module1 */
		kFlexPWM4CaptureSub2 = 106 | 0x100U,					   /**< FlexPWM4 Capture sub-module2 */
		kFlexPWM4CaptureSub3 = 107 | 0x100U,					   /**< FlexPWM4 Capture sub-module3 */
		kFlexPWM4ValueSub0 = 108 | 0x100U,						   /**< FlexPWM4 Value sub-module0 */
		kFlexPWM4ValueSub1 = 109 | 0x100U,						   /**< FlexPWM4 Value sub-module1 */
		kFlexPWM4ValueSub2 = 110 | 0x100U,						   /**< FlexPWM4 Value sub-module2 */
		kFlexPWM4ValueSub3 = 111 | 0x100U,						   /**< FlexPWM4 Value sub-module3 */
		kQTIMER2CaptTimer0 = 112 | 0x100U,						   /**< QTIMER2 Capture timer 0 */
		kQTIMER2CaptTimer1 = 113 | 0x100U,						   /**< QTIMER2 Capture timer 1 */
		kQTIMER2CaptTimer2 = 114 | 0x100U,						   /**< QTIMER2 Capture timer 2 */
		kQTIMER2CaptTimer3 = 115 | 0x100U,						   /**< QTIMER2 Capture timer 3 */
		kQTIMER2Cmpld1Timer0Cmpld2Timer1 = 116 | 0x100U,		   /**< QTIMER2 cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER2Cmpld1Timer1Cmpld2Timer0 = 117 | 0x100U,		   /**< QTIMER2 cmpld1 in timer 1 or cmpld2 in timer 0 */
		kQTIMER2Cmpld1Timer2Cmpld2Timer3 = 118 | 0x100U,		   /**< QTIMER2 cmpld1 in timer 2 or cmpld2 in timer 3 */
		kQTIMER2Cmpld1Timer3Cmpld2Timer2 = 119 | 0x100U,		   /**< QTIMER2 cmpld1 in timer 3 or cmpld2 in timer 2 */
		kQTIMER4CaptTimer0Cmpld1Timer0Cmpld2Timer1 = 120 | 0x100U, /**< QTIMER4 capture timer 0, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER4CaptTimer1Cmpld1Timer0Cmpld2Timer1 = 121 | 0x100U, /**< QTIMER4 capture timer 1, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER4CaptTimer2Cmpld1Timer0Cmpld2Timer1 = 122 | 0x100U, /**< QTIMER4 capture timer 2, cmpld1 in timer 0 or cmpld2 in timer 1 */
		kQTIMER4CaptTimer3Cmpld1Timer0Cmpld2Timer1 = 123 | 0x100U, /**< QTIMER4 capture timer 3, cmpld1 in timer 0 or cmpld2 in timer 1 */
	};
	DMAMux();
	inline void SetSource(RequestSource source, uint8_t channel)
	{
		dma_mux_base->CHCFG[channel] = ((dma_mux_base->CHCFG[channel] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE((uint32_t)source));
		EnableChannel(channel);
	}
	inline void SetPeriodTrigger(uint8_t channel)
	{
		dma_mux_base->CHCFG[channel] = DMAMUX_CHCFG_TRIG(1) | DMAMUX_CHCFG_ENBL(1) | DMAMUX_CHCFG_A_ON_MASK | DMAMUX_CHCFG_SOURCE_MASK;
	}

  private:
	inline void EnableChannel(uint8_t channel)
	{
		dma_mux_base->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK;
	}

	DMAMUX_Type *dma_mux_base;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_DMA_MUX_H_ */
