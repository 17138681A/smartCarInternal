/*
 * xbar.h
 *
 *  Created on: 19 Dec 2018
 *      Author: lch19
 */

#ifndef XBAR_H_
#define XBAR_H_

#include <bitset>
#include "system/pinout/pinout.h"

namespace Driver
{

/*!
 * @addtogroup xbar Inter-Peripheral Crossbar Switch
 * @ingroup driver
 * This module is to provide a flexible crossbar switch function that allows any input (typically from external GPIO or internal module outputs) 
 * to be connected to any output (typically to external GPIO or internal module inputs) under user control. 
 * This is used to allow user configuration of data paths between internal modules and between internal modules and GPIO.
 * @{
 */
/**
 * @brief Inter-Peripheral Crossbar Switch
 */
class XBARA
{
  public:
	enum struct XBAR1InputSignal
	{
		kLogicLow = 0 | 0x100U,				  /*!< Logic low */
		kLogicHigh = 1 | 0x100U,			  /*!< Logic High */
		kIomuxXbarIn02 = 2 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN02 */
		kIomuxXbarIn03 = 3 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN03 */
		kIomuxXbarInout04 = 4 | 0x100U,		  /*!< External pin IOMUX_XBAR_INOUT04 */
		kIomuxXbarInout05 = 5 | 0x100U,		  /*!< External pin IOMUX_XBAR_INOUT05 */
		kIomuxXbarInout06 = 6 | 0x100U,		  /*!< External pin IOMUX_XBAR_INOUT06 */
		kIomuxXbarInout07 = 7 | 0x100U,		  /*!< External pin IOMUX_XBAR_INOUT07 */
		kIomuxXbarInout08 = 8 | 0x100U,		  /*!< External pin IOMUX_XBAR_INOUT08 */
		kIomuxXbarInout09 = 9 | 0x100U,		  /*!< External pin IOMUX_XBAR_INOUT09 */
		kIomuxXbarInout10 = 10 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT10 */
		kIomuxXbarInout11 = 11 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT11 */
		kIomuxXbarInout12 = 12 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT12 */
		kIomuxXbarInout13 = 13 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT13 */
		kIomuxXbarInout14 = 14 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT14 */
		kIomuxXbarInout15 = 15 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT15 */
		kIomuxXbarInout16 = 16 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT16 */
		kIomuxXbarInout17 = 17 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT17 */
		kIomuxXbarInout18 = 18 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT18 */
		kIomuxXbarInout19 = 19 | 0x100U,	  /*!< External pin IOMUX_XBAR_INOUT19 */
		kIomuxXbarIn20 = 20 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN20 */
		kIomuxXbarIn21 = 21 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN21 */
		kIomuxXbarIn22 = 22 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN22 */
		kIomuxXbarIn23 = 23 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN23 */
		kIomuxXbarIn24 = 24 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN24 */
		kIomuxXbarIn25 = 25 | 0x100U,		  /*!< External pin IOMUX_XBAR_IN25 */
		kAcmp1Out = 26 | 0x100U,			  /*!< ACMP1_OUT */
		kAcmp2Out = 27 | 0x100U,			  /*!< ACMP2_OUT */
		kAcmp3Out = 28 | 0x100U,			  /*!< ACMP3_OUT */
		kAcmp4Out = 29 | 0x100U,			  /*!< ACMP4_OUT */
		kRESERVED30 = 30 | 0x100U,			  /*!< Reserved */
		kRESERVED31 = 31 | 0x100U,			  /*!< Reserved */
		kQtimer3Tmr0Output = 32 | 0x100U,	 /*!< Quad Timer3 TMR0 */
		kQtimer3Tmr1Output = 33 | 0x100U,	 /*!< Quad Timer3 TMR1 */
		kQtimer3Tmr2Output = 34 | 0x100U,	 /*!< Quad Timer3 TMR2 */
		kQtimer3Tmr3Output = 35 | 0x100U,	 /*!< Quad Timer3 TMR3 */
		kQtimer4Tmr0Output = 36 | 0x100U,	 /*!< Quad Timer4 TMR0 */
		kQtimer4Tmr1Output = 37 | 0x100U,	 /*!< Quad Timer4 TMR1 */
		kQtimer4Tmr2Output = 38 | 0x100U,	 /*!< Quad Timer4 TMR2 */
		kQtimer4Tmr3Output = 39 | 0x100U,	 /*!< Quad Timer4 TMR3 */
		kFlexpwm1Pwm1OutTrig01 = 40 | 0x100U, /*!< FlexPWM1 PWM1 output trigger01 */
		kFlexpwm1Pwm2OutTrig01 = 41 | 0x100U, /*!< FlexPWM1 PWM2 output trigger01 */
		kFlexpwm1Pwm3OutTrig01 = 42 | 0x100U, /*!< FlexPWM1 PWM3 output trigger01 */
		kFlexpwm1Pwm4OutTrig01 = 43 | 0x100U, /*!< FlexPWM1 PWM4 output trigger01 */
		kFlexpwm2Pwm1OutTrig01 = 44 | 0x100U, /*!< FlexPWM2 PWM1 output trigger01 */
		kFlexpwm2Pwm2OutTrig01 = 45 | 0x100U, /*!< FlexPWM2 PWM2 output trigger01 */
		kFlexpwm2Pwm3OutTrig01 = 46 | 0x100U, /*!< FlexPWM2 PWM3 output trigger01 */
		kFlexpwm2Pwm4OutTrig01 = 47 | 0x100U, /*!< FlexPWM2 PWM4 output trigger01 */
		kFlexpwm3Pwm1OutTrig01 = 48 | 0x100U, /*!< FlexPWM3 PWM1 output trigger01 */
		kFlexpwm3Pwm2OutTrig01 = 49 | 0x100U, /*!< FlexPWM3 PWM2 output trigger01 */
		kFlexpwm3Pwm3OutTrig01 = 50 | 0x100U, /*!< FlexPWM3 PWM3 output trigger01 */
		kFlexpwm3Pwm4OutTrig01 = 51 | 0x100U, /*!< FlexPWM3 PWM4 output trigger01 */
		kFlexpwm4Pwm1OutTrig01 = 52 | 0x100U, /*!< FlexPWM4 PWM1 output trigger01 */
		kFlexpwm4Pwm2OutTrig01 = 53 | 0x100U, /*!< FlexPWM4 PWM2 output trigger01 */
		kFlexpwm4Pwm3OutTrig01 = 54 | 0x100U, /*!< FlexPWM4 PWM3 output trigger01 */
		kFlexpwm4Pwm4OutTrig01 = 55 | 0x100U, /*!< FlexPWM4 PWM4 output trigger01 */
		kPitTrigger0 = 56 | 0x100U,			  /*!< PIT trigger0 */
		kPitTrigger1 = 57 | 0x100U,			  /*!< PIT trigger1 */
		kPitTrigger2 = 58 | 0x100U,			  /*!< PIT trigger2 */
		kPitTrigger3 = 59 | 0x100U,			  /*!< PIT trigger3 */
		kEnc1PosMatch = 60 | 0x100U,		  /*!< Quad decoder1 position match */
		kEnc2PosMatch = 61 | 0x100U,		  /*!< Quad decoder2 position match */
		kEnc3PosMatch = 62 | 0x100U,		  /*!< Quad decoder3 position match */
		kEnc4PosMatch = 63 | 0x100U,		  /*!< Quad decoder4 position match */
		kDmaDone0 = 64 | 0x100U,			  /*!< eDMA done0 */
		kDmaDone1 = 65 | 0x100U,			  /*!< eDMA done1 */
		kDmaDone2 = 66 | 0x100U,			  /*!< eDMA done2 */
		kDmaDone3 = 67 | 0x100U,			  /*!< eDMA done3 */
		kDmaDone4 = 68 | 0x100U,			  /*!< eDMA done4 */
		kDmaDone5 = 69 | 0x100U,			  /*!< eDMA done5 */
		kDmaDone6 = 70 | 0x100U,			  /*!< eDMA done6 */
		kDmaDone7 = 71 | 0x100U,			  /*!< eDMA done7 */
		kAoi1Out0 = 72 | 0x100U,			  /*!< AOI1 output 0 */
		kAoi1Out1 = 73 | 0x100U,			  /*!< AOI1 output 1 */
		kAoi1Out2 = 74 | 0x100U,			  /*!< AOI1 output 2 */
		kAoi1Out3 = 75 | 0x100U,			  /*!< AOI1 output 3 */
		kAoi2Out0 = 76 | 0x100U,			  /*!< AOI2 output 0 */
		kAoi2Out1 = 77 | 0x100U,			  /*!< AOI2 output 1 */
		kAoi2Out2 = 78 | 0x100U,			  /*!< AOI2 output 2 */
		kAoi2Out3 = 79 | 0x100U,			  /*!< AOI2 output 3 */
		kAdcEtcXbar0Coco0 = 80 | 0x100U,	  /*!< ADC_ETC_XBAR0_COCO0 */
		kAdcEtcXbar0Coco1 = 81 | 0x100U,	  /*!< ADC_ETC_XBAR0_COCO1 */
		kAdcEtcXbar0Coco2 = 82 | 0x100U,	  /*!< ADC_ETC_XBAR0_COCO2 */
		kAdcEtcXbar0Coco3 = 83 | 0x100U,	  /*!< ADC_ETC_XBAR0_COCO3 */
		kAdcEtcXbar1Coco0 = 84 | 0x100U,	  /*!< ADC_ETC_XBAR1_COCO0 */
		kAdcEtcXbar1Coco1 = 85 | 0x100U,	  /*!< ADC_ETC_XBAR1_COCO1 */
		kAdcEtcXbar1Coco2 = 86 | 0x100U,	  /*!< ADC_ETC_XBAR1_COCO2 */
		kAdcEtcXbar1Coco3 = 87 | 0x100U,	  /*!< ADC_ETC_XBAR1_COCO3 */
	};
	enum struct XBAR1OutputSignal
	{
		kDmaChMuxReq30 = 0 | 0x100U,	   /*!< XBARA1_OUT0 output assigned to DMA_CH_MUX_REQ30 */
		kDmaChMuxReq31 = 1 | 0x100U,	   /*!< XBARA1_OUT1 output assigned to DMA_CH_MUX_REQ31 */
		kDmaChMuxReq94 = 2 | 0x100U,	   /*!< XBARA1_OUT2 output assigned to DMA_CH_MUX_REQ94 */
		kDmaChMuxReq95 = 3 | 0x100U,	   /*!< XBARA1_OUT3 output assigned to DMA_CH_MUX_REQ95 */
		kIomuxXbarInout04 = 4 | 0x100U,	/*!< XBARA1_OUT4 output assigned to IOMUX_XBAR_INOUT04 */
		kIomuxXbarInout05 = 5 | 0x100U,	/*!< XBARA1_OUT5 output assigned to IOMUX_XBAR_INOUT05 */
		kIomuxXbarInout06 = 6 | 0x100U,	/*!< XBARA1_OUT6 output assigned to IOMUX_XBAR_INOUT06 */
		kIomuxXbarInout07 = 7 | 0x100U,	/*!< XBARA1_OUT7 output assigned to IOMUX_XBAR_INOUT07 */
		kIomuxXbarInout08 = 8 | 0x100U,	/*!< XBARA1_OUT8 output assigned to IOMUX_XBAR_INOUT08 */
		kIomuxXbarInout09 = 9 | 0x100U,	/*!< XBARA1_OUT9 output assigned to IOMUX_XBAR_INOUT09 */
		kIomuxXbarInout10 = 10 | 0x100U,   /*!< XBARA1_OUT10 output assigned to IOMUX_XBAR_INOUT10 */
		kIomuxXbarInout11 = 11 | 0x100U,   /*!< XBARA1_OUT11 output assigned to IOMUX_XBAR_INOUT11 */
		kIomuxXbarInout12 = 12 | 0x100U,   /*!< XBARA1_OUT12 output assigned to IOMUX_XBAR_INOUT12 */
		kIomuxXbarInout13 = 13 | 0x100U,   /*!< XBARA1_OUT13 output assigned to IOMUX_XBAR_INOUT13 */
		kIomuxXbarInout14 = 14 | 0x100U,   /*!< XBARA1_OUT14 output assigned to IOMUX_XBAR_INOUT14 */
		kIomuxXbarInout15 = 15 | 0x100U,   /*!< XBARA1_OUT15 output assigned to IOMUX_XBAR_INOUT15 */
		kIomuxXbarInout16 = 16 | 0x100U,   /*!< XBARA1_OUT16 output assigned to IOMUX_XBAR_INOUT16 */
		kIomuxXbarInout17 = 17 | 0x100U,   /*!< XBARA1_OUT17 output assigned to IOMUX_XBAR_INOUT17 */
		kIomuxXbarInout18 = 18 | 0x100U,   /*!< XBARA1_OUT18 output assigned to IOMUX_XBAR_INOUT18 */
		kIomuxXbarInout19 = 19 | 0x100U,   /*!< XBARA1_OUT19 output assigned to IOMUX_XBAR_INOUT19 */
		kAcmp1Sample = 20 | 0x100U,		   /*!< XBARA1_OUT20 output assigned to ACMP1_SAMPLE */
		kAcmp2Sample = 21 | 0x100U,		   /*!< XBARA1_OUT21 output assigned to ACMP2_SAMPLE */
		kAcmp3Sample = 22 | 0x100U,		   /*!< XBARA1_OUT22 output assigned to ACMP3_SAMPLE */
		kAcmp4Sample = 23 | 0x100U,		   /*!< XBARA1_OUT23 output assigned to ACMP4_SAMPLE */
		kRESERVED24 = 24 | 0x100U,		   /*!< XBARA1_OUT24 output is reserved. */
		kRESERVED25 = 25 | 0x100U,		   /*!< XBARA1_OUT25 output is reserved. */
		kFlexpwm1Exta0 = 26 | 0x100U,	  /*!< XBARA1_OUT26 output assigned to FLEXPWM1_EXTA0 */
		kFlexpwm1Exta1 = 27 | 0x100U,	  /*!< XBARA1_OUT27 output assigned to FLEXPWM1_EXTA1 */
		kFlexpwm1Exta2 = 28 | 0x100U,	  /*!< XBARA1_OUT28 output assigned to FLEXPWM1_EXTA2 */
		kFlexpwm1Exta3 = 29 | 0x100U,	  /*!< XBARA1_OUT29 output assigned to FLEXPWM1_EXTA3 */
		kFlexpwm1ExtSync0 = 30 | 0x100U,   /*!< XBARA1_OUT30 output assigned to FLEXPWM1_EXT_SYNC0 */
		kFlexpwm1ExtSync1 = 31 | 0x100U,   /*!< XBARA1_OUT31 output assigned to FLEXPWM1_EXT_SYNC1 */
		kFlexpwm1ExtSync2 = 32 | 0x100U,   /*!< XBARA1_OUT32 output assigned to FLEXPWM1_EXT_SYNC2 */
		kFlexpwm1ExtSync3 = 33 | 0x100U,   /*!< XBARA1_OUT33 output assigned to FLEXPWM1_EXT_SYNC3 */
		kFlexpwm1ExtClk = 34 | 0x100U,	 /*!< XBARA1_OUT34 output assigned to FLEXPWM1_EXT_CLK */
		kFlexpwm1Fault0 = 35 | 0x100U,	 /*!< XBARA1_OUT35 output assigned to FLEXPWM1_FAULT0 */
		kFlexpwm1Fault1 = 36 | 0x100U,	 /*!< XBARA1_OUT36 output assigned to FLEXPWM1_FAULT1 */
		kFlexpwm1234Fault2 = 37 | 0x100U,  /*!< XBARA1_OUT37 output assigned to FLEXPWM1_2_3_4_FAULT2 */
		kFlexpwm1234Fault3 = 38 | 0x100U,  /*!< XBARA1_OUT38 output assigned to FLEXPWM1_2_3_4_FAULT3 */
		kFlexpwm1ExtForce = 39 | 0x100U,   /*!< XBARA1_OUT39 output assigned to FLEXPWM1_EXT_FORCE */
		kFlexpwm234Exta0 = 40 | 0x100U,	/*!< XBARA1_OUT40 output assigned to FLEXPWM2_3_4_EXTA0 */
		kFlexpwm234Exta1 = 41 | 0x100U,	/*!< XBARA1_OUT41 output assigned to FLEXPWM2_3_4_EXTA1 */
		kFlexpwm234Exta2 = 42 | 0x100U,	/*!< XBARA1_OUT42 output assigned to FLEXPWM2_3_4_EXTA2 */
		kFlexpwm234Exta3 = 43 | 0x100U,	/*!< XBARA1_OUT43 output assigned to FLEXPWM2_3_4_EXTA3 */
		kFlexpwm2ExtSync0 = 44 | 0x100U,   /*!< XBARA1_OUT44 output assigned to FLEXPWM2_EXT_SYNC0 */
		kFlexpwm2ExtSync1 = 45 | 0x100U,   /*!< XBARA1_OUT45 output assigned to FLEXPWM2_EXT_SYNC1 */
		kFlexpwm2ExtSync2 = 46 | 0x100U,   /*!< XBARA1_OUT46 output assigned to FLEXPWM2_EXT_SYNC2 */
		kFlexpwm2ExtSync3 = 47 | 0x100U,   /*!< XBARA1_OUT47 output assigned to FLEXPWM2_EXT_SYNC3 */
		kFlexpwm234ExtClk = 48 | 0x100U,   /*!< XBARA1_OUT48 output assigned to FLEXPWM2_3_4_EXT_CLK */
		kFlexpwm2Fault0 = 49 | 0x100U,	 /*!< XBARA1_OUT49 output assigned to FLEXPWM2_FAULT0 */
		kFlexpwm2Fault1 = 50 | 0x100U,	 /*!< XBARA1_OUT50 output assigned to FLEXPWM2_FAULT1 */
		kFlexpwm2ExtForce = 51 | 0x100U,   /*!< XBARA1_OUT51 output assigned to FLEXPWM2_EXT_FORCE */
		kFlexpwm3ExtSync0 = 52 | 0x100U,   /*!< XBARA1_OUT52 output assigned to FLEXPWM3_EXT_SYNC0 */
		kFlexpwm3ExtSync1 = 53 | 0x100U,   /*!< XBARA1_OUT53 output assigned to FLEXPWM3_EXT_SYNC1 */
		kFlexpwm3ExtSync2 = 54 | 0x100U,   /*!< XBARA1_OUT54 output assigned to FLEXPWM3_EXT_SYNC2 */
		kFlexpwm3ExtSync3 = 55 | 0x100U,   /*!< XBARA1_OUT55 output assigned to FLEXPWM3_EXT_SYNC3 */
		kFlexpwm3Fault0 = 56 | 0x100U,	 /*!< XBARA1_OUT56 output assigned to FLEXPWM3_FAULT0 */
		kFlexpwm3Fault1 = 57 | 0x100U,	 /*!< XBARA1_OUT57 output assigned to FLEXPWM3_FAULT1 */
		kFlexpwm3ExtForce = 58 | 0x100U,   /*!< XBARA1_OUT58 output assigned to FLEXPWM3_EXT_FORCE */
		kFlexpwm4ExtSync0 = 59 | 0x100U,   /*!< XBARA1_OUT59 output assigned to FLEXPWM4_EXT_SYNC0 */
		kFlexpwm4ExtSync1 = 60 | 0x100U,   /*!< XBARA1_OUT60 output assigned to FLEXPWM4_EXT_SYNC1 */
		kFlexpwm4ExtSync2 = 61 | 0x100U,   /*!< XBARA1_OUT61 output assigned to FLEXPWM4_EXT_SYNC2 */
		kFlexpwm4ExtSync3 = 62 | 0x100U,   /*!< XBARA1_OUT62 output assigned to FLEXPWM4_EXT_SYNC3 */
		kFlexpwm4Fault0 = 63 | 0x100U,	 /*!< XBARA1_OUT63 output assigned to FLEXPWM4_FAULT0 */
		kFlexpwm4Fault1 = 64 | 0x100U,	 /*!< XBARA1_OUT64 output assigned to FLEXPWM4_FAULT1 */
		kFlexpwm4ExtForce = 65 | 0x100U,   /*!< XBARA1_OUT65 output assigned to FLEXPWM4_EXT_FORCE */
		kEnc1PhaseAInput = 66 | 0x100U,	/*!< XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
		kEnc1PhaseBInput = 67 | 0x100U,	/*!< XBARA1_OUT67 output assigned to ENC1_PHASE_B_INPUT */
		kEnc1Index = 68 | 0x100U,		   /*!< XBARA1_OUT68 output assigned to ENC1_INDEX */
		kEnc1Home = 69 | 0x100U,		   /*!< XBARA1_OUT69 output assigned to ENC1_HOME */
		kEnc1Trigger = 70 | 0x100U,		   /*!< XBARA1_OUT70 output assigned to ENC1_TRIGGER */
		kEnc2PhaseAInput = 71 | 0x100U,	/*!< XBARA1_OUT71 output assigned to ENC2_PHASE_A_INPUT */
		kEnc2PhaseBInput = 72 | 0x100U,	/*!< XBARA1_OUT72 output assigned to ENC2_PHASE_B_INPUT */
		kEnc2Index = 73 | 0x100U,		   /*!< XBARA1_OUT73 output assigned to ENC2_INDEX */
		kEnc2Home = 74 | 0x100U,		   /*!< XBARA1_OUT74 output assigned to ENC2_HOME */
		kEnc2Trigger = 75 | 0x100U,		   /*!< XBARA1_OUT75 output assigned to ENC2_TRIGGER */
		kEnc3PhaseAInput = 76 | 0x100U,	/*!< XBARA1_OUT76 output assigned to ENC3_PHASE_A_INPUT */
		kEnc3PhaseBInput = 77 | 0x100U,	/*!< XBARA1_OUT77 output assigned to ENC3_PHASE_B_INPUT */
		kEnc3Index = 78 | 0x100U,		   /*!< XBARA1_OUT78 output assigned to ENC3_INDEX */
		kEnc3Home = 79 | 0x100U,		   /*!< XBARA1_OUT79 output assigned to ENC3_HOME */
		kEnc3Trigger = 80 | 0x100U,		   /*!< XBARA1_OUT80 output assigned to ENC3_TRIGGER */
		kEnc4PhaseAInput = 81 | 0x100U,	/*!< XBARA1_OUT81 output assigned to ENC4_PHASE_A_INPUT */
		kEnc4PhaseBInput = 82 | 0x100U,	/*!< XBARA1_OUT82 output assigned to ENC4_PHASE_B_INPUT */
		kEnc4Index = 83 | 0x100U,		   /*!< XBARA1_OUT83 output assigned to ENC4_INDEX */
		kEnc4Home = 84 | 0x100U,		   /*!< XBARA1_OUT84 output assigned to ENC4_HOME */
		kEnc4Trigger = 85 | 0x100U,		   /*!< XBARA1_OUT85 output assigned to ENC4_TRIGGER */
		kQtimer1Tmr0Input = 86 | 0x100U,   /*!< XBARA1_OUT86 output assigned to QTIMER1_TMR0_INPUT */
		kQtimer1Tmr1Input = 87 | 0x100U,   /*!< XBARA1_OUT87 output assigned to QTIMER1_TMR1_INPUT */
		kQtimer1Tmr2Input = 88 | 0x100U,   /*!< XBARA1_OUT88 output assigned to QTIMER1_TMR2_INPUT */
		kQtimer1Tmr3Input = 89 | 0x100U,   /*!< XBARA1_OUT89 output assigned to QTIMER1_TMR3_INPUT */
		kQtimer2Tmr0Input = 90 | 0x100U,   /*!< XBARA1_OUT90 output assigned to QTIMER2_TMR0_INPUT */
		kQtimer2Tmr1Input = 91 | 0x100U,   /*!< XBARA1_OUT91 output assigned to QTIMER2_TMR1_INPUT */
		kQtimer2Tmr2Input = 92 | 0x100U,   /*!< XBARA1_OUT92 output assigned to QTIMER2_TMR2_INPUT */
		kQtimer2Tmr3Input = 93 | 0x100U,   /*!< XBARA1_OUT93 output assigned to QTIMER2_TMR3_INPUT */
		kQtimer3Tmr0Input = 94 | 0x100U,   /*!< XBARA1_OUT94 output assigned to QTIMER3_TMR0_INPUT */
		kQtimer3Tmr1Input = 95 | 0x100U,   /*!< XBARA1_OUT95 output assigned to QTIMER3_TMR1_INPUT */
		kQtimer3Tmr2Input = 96 | 0x100U,   /*!< XBARA1_OUT96 output assigned to QTIMER3_TMR2_INPUT */
		kQtimer3Tmr3Input = 97 | 0x100U,   /*!< XBARA1_OUT97 output assigned to QTIMER3_TMR3_INPUT */
		kQtimer4Tmr0Input = 98 | 0x100U,   /*!< XBARA1_OUT98 output assigned to QTIMER4_TMR0_INPUT */
		kQtimer4Tmr1Input = 99 | 0x100U,   /*!< XBARA1_OUT99 output assigned to QTIMER4_TMR1_INPUT */
		kQtimer4Tmr2Input = 100 | 0x100U,  /*!< XBARA1_OUT100 output assigned to QTIMER4_TMR2_INPUT */
		kQtimer4Tmr3Input = 101 | 0x100U,  /*!< XBARA1_OUT101 output assigned to QTIMER4_TMR3_INPUT */
		kEwmEwmIn = 102 | 0x100U,		   /*!< XBARA1_OUT102 output assigned to EWM_EWM_IN */
		kAdcEtcXbar0Trig0 = 103 | 0x100U,  /*!< XBARA1_OUT103 output assigned to ADC_ETC_XBAR0_TRIG0 */
		kAdcEtcXbar0Trig1 = 104 | 0x100U,  /*!< XBARA1_OUT104 output assigned to ADC_ETC_XBAR0_TRIG1 */
		kAdcEtcXbar0Trig2 = 105 | 0x100U,  /*!< XBARA1_OUT105 output assigned to ADC_ETC_XBAR0_TRIG2 */
		kAdcEtcXbar0Trig3 = 106 | 0x100U,  /*!< XBARA1_OUT106 output assigned to ADC_ETC_XBAR0_TRIG3 */
		kAdcEtcXbar1Trig0 = 107 | 0x100U,  /*!< XBARA1_OUT107 output assigned to ADC_ETC_XBAR1_TRIG0 */
		kAdcEtcXbar1Trig1 = 108 | 0x100U,  /*!< XBARA1_OUT108 output assigned to ADC_ETC_XBAR1_TRIG1 */
		kAdcEtcXbar1Trig2 = 109 | 0x100U,  /*!< XBARA1_OUT109 output assigned to ADC_ETC_XBAR1_TRIG2 */
		kAdcEtcXbar1Trig3 = 110 | 0x100U,  /*!< XBARA1_OUT110 output assigned to ADC_ETC_XBAR1_TRIG3 */
		kLpi2c1TrgInput = 111 | 0x100U,	/*!< XBARA1_OUT111 output assigned to LPI2C1_TRG_INPUT */
		kLpi2c2TrgInput = 112 | 0x100U,	/*!< XBARA1_OUT112 output assigned to LPI2C2_TRG_INPUT */
		kLpi2c3TrgInput = 113 | 0x100U,	/*!< XBARA1_OUT113 output assigned to LPI2C3_TRG_INPUT */
		kLpi2c4TrgInput = 114 | 0x100U,	/*!< XBARA1_OUT114 output assigned to LPI2C4_TRG_INPUT */
		kLpspi1TrgInput = 115 | 0x100U,	/*!< XBARA1_OUT115 output assigned to LPSPI1_TRG_INPUT */
		kLpspi2TrgInput = 116 | 0x100U,	/*!< XBARA1_OUT116 output assigned to LPSPI2_TRG_INPUT */
		kLpspi3TrgInput = 117 | 0x100U,	/*!< XBARA1_OUT117 output assigned to LPSPI3_TRG_INPUT */
		kLpspi4TrgInput = 118 | 0x100U,	/*!< XBARA1_OUT118 output assigned to LPSPI4_TRG_INPUT */
		kLpuart1TrgInput = 119 | 0x100U,   /*!< XBARA1_OUT119 output assigned to LPUART1_TRG_INPUT */
		kLpuart2TrgInput = 120 | 0x100U,   /*!< XBARA1_OUT120 output assigned to LPUART2_TRG_INPUT */
		kLpuart3TrgInput = 121 | 0x100U,   /*!< XBARA1_OUT121 output assigned to LPUART3_TRG_INPUT */
		kLpuart4TrgInput = 122 | 0x100U,   /*!< XBARA1_OUT122 output assigned to LPUART4_TRG_INPUT */
		kLpuart5TrgInput = 123 | 0x100U,   /*!< XBARA1_OUT123 output assigned to LPUART5_TRG_INPUT */
		kLpuart6TrgInput = 124 | 0x100U,   /*!< XBARA1_OUT124 output assigned to LPUART6_TRG_INPUT */
		kLpuart7TrgInput = 125 | 0x100U,   /*!< XBARA1_OUT125 output assigned to LPUART7_TRG_INPUT */
		kLpuart8TrgInput = 126 | 0x100U,   /*!< XBARA1_OUT126 output assigned to LPUART8_TRG_INPUT */
		kFlexio1TriggerIn0 = 127 | 0x100U, /*!< XBARA1_OUT127 output assigned to FLEXIO1_TRIGGER_IN0 */
		kFlexio1TriggerIn1 = 128 | 0x100U, /*!< XBARA1_OUT128 output assigned to FLEXIO1_TRIGGER_IN1 */
		kFlexio2TriggerIn0 = 129 | 0x100U, /*!< XBARA1_OUT129 output assigned to FLEXIO2_TRIGGER_IN0 */
		kFlexio2TriggerIn1 = 130 | 0x100U, /*!< XBARA1_OUT130 output assigned to FLEXIO2_TRIGGER_IN1 */
	};

	/**
	 * @brief Assign the external pin as XBAR In/InOut pin, so other module can get or ouput signal out the chip through XBAR
	 * 
	 * @param pin_name: 	 pin name of the mux pin
	 * @param input_signal:  it will assign the input signal name of this pin to this variable
	 * @param output_signal: it will assign the output signal name of this pin to this variable
	 * 
	 * @return true:  success
	 * @return false: fail
	 */
	static bool OpenPin(System::Pinout::Name pin_name, XBAR1InputSignal &input_signal, XBAR1OutputSignal &output_signal);

	/**
	 * @brief Connect the two signal together
	 * 
	 * @param input_signal:  input signal that need to be connect
	 * @param output_signal: output signal that need to be connect
	 * 
	 * @return true:  two signal wire connected successfully
	 * @return false: two signal wire fail to connect, this may caused by one of the two signal had been connected before
	 */
	static bool Connect(XBAR1InputSignal input_signal, XBAR1OutputSignal output_signal);

  private:
	static void Init();

	static uint32_t output_used[5];
};
/** @}*/

} // namespace Driver

#endif /* XBAR_H_ */
