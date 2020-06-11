/*
 * system.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: LeeChunHei
 */

#include "system/system.h"
#include "system/cmsis/access_layer/access_layer.h"
#include "system/clock.h"
#include "system/systick.h"
#include "system/temp_monitor.h"
#include "../config/config.h"

#include <stdio.h>

namespace System
{

const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN =
	{
		.loopDivider = 100, /* PLL loop divider, Fout = Fin * 50 */
		.src = 0,			/* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_sys_pll_config_t sysPllConfig_BOARD_BootClockRUN =
	{
		.loopDivider = 1, /* PLL loop divider, Fout = Fin * ( 20 + loopDivider*2 + numerator / denominator ) */
		.numerator = 0,   /* 30 bit numerator of fractional loop divider */
		.denominator = 1, /* 30 bit denominator of fractional loop divider */
		.src = 0,		  /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_usb_pll_config_t usb1PllConfig_BOARD_BootClockRUN =
	{
		.loopDivider = 0, /* PLL loop divider, Fout = Fin * 20 */
		.src = 0,		  /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
const clock_enet_pll_config_t enetPllConfig_BOARD_BootClockRUN =
	{
		.enableClkOutput = true,	/* Enable the PLL providing the ENET 125MHz reference clock */
		.enableClkOutput25M = true, /* Enable the PLL providing the ENET 25MHz reference clock */
		.loopDivider = 3,			/* Set frequency of ethernet reference clock to 125 MHz */
		.src = 0,					/* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};

uint32_t system_core_clock = 0; /*!< Core clock */
uint32_t ahb_clock = 0;			/*!< AHB clock */
uint32_t semc_clock = 0;		/*!< SEMC clock */
uint32_t ipg_clock = 0;			/*!< IPG clock */

uint32_t osc_clock = 0; /*!< OSC clock selected by PMU_LOWPWR_CTRL[OSC_SEL]. */
uint32_t rtc_clock = 0; /*!< RTC clock. (RTCCLK) */

uint32_t arm_pll_clock = 0; /*!< ARMPLLCLK. */

uint32_t usb1_pll_clock = 0;	  /*!< USB1PLLCLK. */
uint32_t usb1_pll_pfd0_clock = 0; /*!< USB1PLLPDF0CLK. */
uint32_t usb1_pll_pfd1_clock = 0; /*!< USB1PLLPFD1CLK. */
uint32_t usb1_pll_pfd2_clock = 0; /*!< USB1PLLPFD2CLK. */
uint32_t usb1_pll_pfd3_clock = 0; /*!< USB1PLLPFD3CLK. */

uint32_t usb2_pll_clock = 0; /*!< USB2PLLCLK. */

uint32_t sys_pll_clock = 0;		 /*!< SYSPLLCLK. */
uint32_t sys_pll_pfd0_clock = 0; /*!< SYSPLLPDF0CLK. */
uint32_t sys_pll_pfd1_clock = 0; /*!< SYSPLLPFD1CLK. */
uint32_t sys_pll_pfd2_clock = 0; /*!< SYSPLLPFD2CLK. */
uint32_t sys_pll_pfd3_clock = 0; /*!< SYSPLLPFD3CLK. */

uint32_t enet_pll0_clock = 0; /*!< Enet PLLCLK ref_enetpll0. */
uint32_t enet_pll1_clock = 0; /*!< Enet PLLCLK ref_enetpll1. */

uint32_t audio_pll_clock = 0; /*!< Audio PLLCLK. */
uint32_t video_pll_clock = 0; /*!< Video PLLCLK. */

const char welcome_string[129] =
	"<<=======================================>>\n  RTLib (Library for i.MX RT series chip)\n        Designed by Lee Chun Hei (Leslie)\n";
const char startup_string_template[175] =
	"  <<System Info>>\n    Core Clock: %dMHz\n    ABH Clock:  %dMHz\n    SEMC Clock: %dMHz\n    IPG Clock:  %dMHz\n    Chip Temp:  %.2f°C\n<<=======================================>>\n";

void InitSystem()
{
	/* Disable I cache and D cache */
	if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
	{
		SCB_DisableICache();
	}
	if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
	{
		SCB_DisableDCache();
	}
	/* Disable MPU */
	ARM_MPU_Disable();

	/* MPU configure:
	 * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable, SubRegionDisable, Size)
	 * API in core_cm7.h.
	 * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches disabled.
	 * param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
	 *      Use MACROS defined in core_cm7.h: ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
	 * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
	 *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
	 *     0             x           0           0             Strongly Ordered    shareable
	 *     0             x           0           1              Device             shareable
	 *     0             0           1           0              Normal             not shareable   Outer and inner write through no write allocate
	 *     0             0           1           1              Normal             not shareable   Outer and inner write back no write allocate
	 *     0             1           1           0              Normal             shareable       Outer and inner write through no write allocate
	 *     0             1           1           1              Normal             shareable       Outer and inner write back no write allocate
	 *     1             0           0           0              Normal             not shareable   outer and inner noncache
	 *     1             1           0           0              Normal             shareable       outer and inner noncache
	 *     1             0           1           1              Normal             not shareable   outer and inner write back write/read acllocate
	 *     1             1           1           1              Normal             shareable       outer and inner write back write/read acllocate
	 *     2             x           0           0              Device              not shareable
	 *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache policy.
	 *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
	 * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
	 * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in core_cm7.h.
	 */

	/* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
	MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

	/* Region 1 setting: Memory with Device type, not shareable,  non-cacheable. */
	MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

	/* Region 2 setting */
	/* Setting Memory with Normal type, not shareable, outer/inner write back. */
	MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512MB);

	/* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
	MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

	/* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

	/* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

	/* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

	/* The define sets the cacheable memory to shareable,
	 * this suggestion is referred from chapter 2.2.1 Memory regions,
	 * types and attributes in Cortex-M7 Devices, Generic User Guide */
#if defined(SDRAM_IS_SHAREABLE)
	/* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#else
	/* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_16MB);
#endif

	/* Region 8 setting, set last 2MB of SDRAM can't be accessed by cache, glocal variables which are not expected to be
	 * accessed by cache can be put here */
	/* Memory with Normal type, not shareable, non-cacheable */
	MPU->RBAR = ARM_MPU_RBAR(8, 0x81000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_16MB);

	/* Enable MPU */
	ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

	/* Enable I cache and D cache */
	SCB_EnableDCache();
	SCB_EnableICache();

	/* Init RTC OSC clock frequency. */
	CLOCK_SetRtcXtalFreq(32768U);
	/* Enable 1MHz clock output. */
	XTALOSC24M->OSC_CONFIG2 |= XTALOSC24M_OSC_CONFIG2_ENABLE_1M_MASK;
	/* Use free 1MHz clock output. */
	XTALOSC24M->OSC_CONFIG2 &= ~XTALOSC24M_OSC_CONFIG2_MUX_1M_MASK;
	/* Set XTAL 24MHz clock frequency. */
	CLOCK_SetXtalFreq(24000000U);
	/* Enable XTAL 24MHz clock source. */
	CLOCK_InitExternalClk(0);
	/* Enable internal RC. */
	CLOCK_InitRcOsc24M();
	/* Switch clock source to external OSC. */
	CLOCK_SwitchOsc(kCLOCK_XtalOsc);
	/* Set Oscillator ready counter value. */
	CCM->CCR = (CCM->CCR & (~CCM_CCR_OSCNT_MASK)) | CCM_CCR_OSCNT(127);
	/* Setting PeriphClk2Mux and PeriphMux to provide stable clock before PLLs are initialed */
	CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1); /* Set PERIPH_CLK2 MUX to OSC */
	CLOCK_SetMux(kCLOCK_PeriphMux, 1);	 /* Set PERIPH_CLK MUX to PERIPH_CLK2 */
	/* Setting the VDD_SOC to 1.5V. It is necessary to config AHB to 600Mhz. */
	DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x12);
	/* Waiting for DCDC_STS_DC_OK bit is asserted */
	while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
	{
	}
	/* Init ARM PLL. */
	CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);
	/* In SDK projects, SDRAM (configured by SEMC) will be initialized in either debug script or dcd.
	 * With this macro SKIP_SYSCLK_INIT, system pll (selected to be SEMC source clock in SDK projects) will be left unchanged.
	 * Note: If another clock source is selected for SEMC, user may want to avoid changing that clock as well.*/
#ifndef SKIP_SYSCLK_INIT
	/* Init System PLL. */
	CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
	/* Init System pfd0. */
	CLOCK_InitSysPfd(kCLOCK_Pfd0, 27);
	/* Init System pfd1. */
	CLOCK_InitSysPfd(kCLOCK_Pfd1, 24);
	/* Init System pfd2. */
	CLOCK_InitSysPfd(kCLOCK_Pfd2, 24);
	/* Init System pfd3. */
	CLOCK_InitSysPfd(kCLOCK_Pfd3, 16);
	/* Disable pfd offset. */
	CCM_ANALOG->PLL_SYS &= ~CCM_ANALOG_PLL_SYS_PFD_OFFSET_EN_MASK;
#endif
	/* In SDK projects, external flash (configured by FLEXSPI) will be initialized by dcd.
	 * With this macro XIP_EXTERNAL_FLASH, usb1 pll (selected to be FLEXSPI clock source in SDK projects) will be left unchanged.
	 * Note: If another clock source is selected for FLEXSPI, user may want to avoid changing that clock as well.*/
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
	/* Init Usb1 PLL. */
	CLOCK_InitUsb1Pll(&usb1PllConfig_BOARD_BootClockRUN);
	/* Init Usb1 pfd0. */
	CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);
	/* Init Usb1 pfd1. */
	CLOCK_InitUsb1Pfd(kCLOCK_Pfd1, 16);
	/* Init Usb1 pfd2. */
	CLOCK_InitUsb1Pfd(kCLOCK_Pfd2, 22);
	/* Init Usb1 pfd3. */
	CLOCK_InitUsb1Pfd(kCLOCK_Pfd3, 19);
	/* Disable Usb1 PLL output for USBPHY1. */
	CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
#endif
	/* DeInit Audio PLL. */
	CLOCK_DeinitAudioPll();
	/* Bypass Audio PLL. */
	CLOCK_SetPllBypass(CCM_ANALOG, kCLOCK_PllAudio, 1);
	/* Set divider for Audio PLL. */
	CCM_ANALOG->MISC2 &= ~CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK;
	CCM_ANALOG->MISC2 &= ~CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK;
	/* Enable Audio PLL output. */
	CCM_ANALOG->PLL_AUDIO |= CCM_ANALOG_PLL_AUDIO_ENABLE_MASK;
	/* DeInit Video PLL. */
	CLOCK_DeinitVideoPll();
	/* Bypass Video PLL. */
	CCM_ANALOG->PLL_VIDEO |= CCM_ANALOG_PLL_VIDEO_BYPASS_MASK;
	/* Set divider for Video PLL. */
	CCM_ANALOG->MISC2 = (CCM_ANALOG->MISC2 & (~CCM_ANALOG_MISC2_VIDEO_DIV_MASK)) | CCM_ANALOG_MISC2_VIDEO_DIV(0);
	/* Enable Video PLL output. */
	CCM_ANALOG->PLL_VIDEO |= CCM_ANALOG_PLL_VIDEO_ENABLE_MASK;
	/* Init Enet PLL. */
	CLOCK_InitEnetPll(&enetPllConfig_BOARD_BootClockRUN);
	/* Disable pfd offset. */
	CCM_ANALOG->PLL_ENET &= ~CCM_ANALOG_PLL_ENET_PFD_OFFSET_EN_MASK;
	/* DeInit Usb2 PLL. */
	CLOCK_DeinitUsb2Pll();
	/* Bypass Usb2 PLL. */
	CLOCK_SetPllBypass(CCM_ANALOG, kCLOCK_PllUsb2, 1);
	/* Enable Usb2 PLL output. */
	CCM_ANALOG->PLL_USB2 |= CCM_ANALOG_PLL_USB2_ENABLE_MASK;
	/* Set AHB_PODF. */
	CLOCK_SetDiv(kCLOCK_AhbDiv, 0);
	/* Set IPG_PODF. */
	CLOCK_SetDiv(kCLOCK_IpgDiv, 3);
	/* Set ARM_PODF. */
	CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
	/* Set preperiph clock source. */
	CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);
	/* Set periph clock source. */
	CLOCK_SetMux(kCLOCK_PeriphMux, 0);
	/* Set PERIPH_CLK2_PODF. */
	CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
	/* Set periph clock2 clock source. */
	CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0);
	/* Set PERCLK_PODF. */
	CLOCK_SetDiv(kCLOCK_PerclkDiv, 1);
	/* Set per clock source. */
	CLOCK_SetMux(kCLOCK_PerclkMux, 0);
	/* Set USDHC1_PODF. */
	CLOCK_SetDiv(kCLOCK_Usdhc1Div, 1);
	/* Set Usdhc1 clock source. */
	CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0);
	/* Set USDHC2_PODF. */
	CLOCK_SetDiv(kCLOCK_Usdhc2Div, 1);
	/* Set Usdhc2 clock source. */
	CLOCK_SetMux(kCLOCK_Usdhc2Mux, 0);
	/* In SDK projects, SDRAM (configured by SEMC) will be initialized in either debug script or dcd.
	 * With this macro SKIP_SYSCLK_INIT, system pll (selected to be SEMC source clock in SDK projects) will be left unchanged.
	 * Note: If another clock source is selected for SEMC, user may want to avoid changing that clock as well.*/
#ifndef SKIP_SYSCLK_INIT
	/* Set SEMC_PODF. */
	CLOCK_SetDiv(kCLOCK_SemcDiv, 3);
	/* Set Semc alt clock source. */
	CLOCK_SetMux(kCLOCK_SemcAltMux, 0);
	/* Set Semc clock source. */
	CLOCK_SetMux(kCLOCK_SemcMux, 0);
#endif
	/* In SDK projects, external flash (configured by FLEXSPI) will be initialized by dcd.
	 * With this macro XIP_EXTERNAL_FLASH, usb1 pll (selected to be FLEXSPI clock source in SDK projects) will be left unchanged.
	 * Note: If another clock source is selected for FLEXSPI, user may want to avoid changing that clock as well.*/
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
	/* Set FLEXSPI_PODF. */
	CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);
	/* Set Flexspi clock source. */
	CLOCK_SetMux(kCLOCK_FlexspiMux, 2);
#endif
	/* Set CSI_PODF. */
	CLOCK_SetDiv(kCLOCK_CsiDiv, 4);
	/* Set Csi clock source. */
	CLOCK_SetMux(kCLOCK_CsiMux, 1);
	/* Set LPSPI_PODF. */
	CLOCK_SetDiv(kCLOCK_LpspiDiv, 2);
	/* Set Lpspi clock source. */
	CLOCK_SetMux(kCLOCK_LpspiMux, 1);
	/* Set TRACE_PODF. */
	CLOCK_SetDiv(kCLOCK_TraceDiv, 2);
	/* Set Trace clock source. */
	CLOCK_SetMux(kCLOCK_TraceMux, 3);
	/* Set SAI1_CLK_PRED. */
	CLOCK_SetDiv(kCLOCK_Sai1PreDiv, 0);
	/* Set SAI1_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Sai1Div, 5);
	/* Set Sai1 clock source. */
	CLOCK_SetMux(kCLOCK_Sai1Mux, 0);
	/* Set SAI2_CLK_PRED. */
	CLOCK_SetDiv(kCLOCK_Sai2PreDiv, 0);
	/* Set SAI2_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Sai2Div, 5);
	/* Set Sai2 clock source. */
	CLOCK_SetMux(kCLOCK_Sai2Mux, 0);
	/* Set SAI3_CLK_PRED. */
	CLOCK_SetDiv(kCLOCK_Sai3PreDiv, 2);
	/* Set SAI3_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Sai3Div, 1);
	/* Set Sai3 clock source. */
	CLOCK_SetMux(kCLOCK_Sai3Mux, 0);
	/* Set LPI2C_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);
	/* Set Lpi2c clock source. */
	CLOCK_SetMux(kCLOCK_Lpi2cMux, 0);
	/* Set CAN_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_CanDiv, 0);
	/* Set Can clock source. */
	CLOCK_SetMux(kCLOCK_CanMux, 0);
	/* Set UART_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_UartDiv, 0);
	/* Set Uart clock source. */
	CLOCK_SetMux(kCLOCK_UartMux, 0);
	/* Set LCDIF_PRED. */
	CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 7);
	/* Set LCDIF_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_LcdifDiv, 0);
	/* Set Lcdif pre clock source. */
	CLOCK_SetMux(kCLOCK_LcdifPreMux, 5);
	/* Set SPDIF0_CLK_PRED. */
	CLOCK_SetDiv(kCLOCK_Spdif0PreDiv, 1);
	/* Set SPDIF0_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Spdif0Div, 3);
	/* Set Spdif clock source. */
	CLOCK_SetMux(kCLOCK_SpdifMux, 3);
	/* Set FLEXIO1_CLK_PRED. */
	CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, 1);
	/* Set FLEXIO1_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Flexio1Div, 1);
	/* Set Flexio1 clock source. */
	CLOCK_SetMux(kCLOCK_Flexio1Mux, 3);
	/* Set FLEXIO2_CLK_PRED. */
	CLOCK_SetDiv(kCLOCK_Flexio2PreDiv, 1);
	/* Set FLEXIO2_CLK_PODF. */
	CLOCK_SetDiv(kCLOCK_Flexio2Div, 1);
	/* Set Flexio2 clock source. */
	CLOCK_SetMux(kCLOCK_Flexio2Mux, 3);
	/* Set Pll3 sw clock source. */
	CLOCK_SetMux(kCLOCK_Pll3SwMux, 0);
	/* Set lvds1 clock source. */
	CCM_ANALOG->MISC1 = (CCM_ANALOG->MISC1 & (~CCM_ANALOG_MISC1_LVDS1_CLK_SEL_MASK)) | CCM_ANALOG_MISC1_LVDS1_CLK_SEL(0);
	/* Set clock out1 divider. */
	CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO1_DIV_MASK)) | CCM_CCOSR_CLKO1_DIV(0);
	/* Set clock out1 source. */
	CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO1_SEL_MASK)) | CCM_CCOSR_CLKO1_SEL(1);
	/* Set clock out2 divider. */
	CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO2_DIV_MASK)) | CCM_CCOSR_CLKO2_DIV(0);
	/* Set clock out2 source. */
	CCM->CCOSR = (CCM->CCOSR & (~CCM_CCOSR_CLKO2_SEL_MASK)) | CCM_CCOSR_CLKO2_SEL(18);
	/* Set clock out1 drives clock out1. */
	CCM->CCOSR &= ~CCM_CCOSR_CLK_OUT_SEL_MASK;
	/* Disable clock out1. */
	CCM->CCOSR &= ~CCM_CCOSR_CLKO1_EN_MASK;
	/* Disable clock out2. */
	CCM->CCOSR &= ~CCM_CCOSR_CLKO2_EN_MASK;
	/* Set Clock variable. */
	system_core_clock = CLOCK_GetCpuClkFreq();
	ahb_clock = CLOCK_GetFreq((clock_name_t)1);  /*!< AHB clock */
	semc_clock = CLOCK_GetFreq((clock_name_t)2); /*!< SEMC clock */
	ipg_clock = CLOCK_GetFreq((clock_name_t)3);  /*!< IPG clock */

	osc_clock = CLOCK_GetFreq((clock_name_t)4); /*!< OSC clock selected by PMU_LOWPWR_CTRL[OSC_SEL]. */
	rtc_clock = CLOCK_GetFreq((clock_name_t)5); /*!< RTC clock. (RTCCLK) */

	arm_pll_clock = CLOCK_GetFreq((clock_name_t)6); /*!< ARMPLLCLK. */

	usb1_pll_clock = CLOCK_GetFreq((clock_name_t)7);	   /*!< USB1PLLCLK. */
	usb1_pll_pfd0_clock = CLOCK_GetFreq((clock_name_t)8);  /*!< USB1PLLPDF0CLK. */
	usb1_pll_pfd1_clock = CLOCK_GetFreq((clock_name_t)9);  /*!< USB1PLLPFD1CLK. */
	usb1_pll_pfd2_clock = CLOCK_GetFreq((clock_name_t)10); /*!< USB1PLLPFD2CLK. */
	usb1_pll_pfd3_clock = CLOCK_GetFreq((clock_name_t)11); /*!< USB1PLLPFD3CLK. */

	usb2_pll_clock = CLOCK_GetFreq((clock_name_t)12); /*!< USB2PLLCLK. */

	sys_pll_clock = CLOCK_GetFreq((clock_name_t)13);	  /*!< SYSPLLCLK. */
	sys_pll_pfd0_clock = CLOCK_GetFreq((clock_name_t)14); /*!< SYSPLLPDF0CLK. */
	sys_pll_pfd1_clock = CLOCK_GetFreq((clock_name_t)15); /*!< SYSPLLPFD1CLK. */
	sys_pll_pfd2_clock = CLOCK_GetFreq((clock_name_t)16); /*!< SYSPLLPFD2CLK. */
	sys_pll_pfd3_clock = CLOCK_GetFreq((clock_name_t)17); /*!< SYSPLLPFD3CLK. */

	enet_pll0_clock = CLOCK_GetFreq((clock_name_t)18); /*!< Enet PLLCLK ref_enetpll0. */
	enet_pll1_clock = CLOCK_GetFreq((clock_name_t)19); /*!< Enet PLLCLK ref_enetpll1. */

	audio_pll_clock = CLOCK_GetFreq((clock_name_t)20); /*!< Audio PLLCLK. */
	video_pll_clock = CLOCK_GetFreq((clock_name_t)21); /*!< Video PLLCLK. */

	TempMonitor::Init();

	printf(welcome_string);
	printf("\n");
	char startup_string[400] = "";
	sprintf(startup_string,
			startup_string_template,
			(int)system_core_clock / 1000000, (int)ahb_clock / 1000000, (int)semc_clock / 1000000, (int)ipg_clock / 1000000, TempMonitor::GetTemperature());
	printf(startup_string);

	Systick::Init();
}

} // namespace System
