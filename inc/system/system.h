/*
 * system.h
 *
 *  Created on: Aug 26, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_SYSTEM_SYSTEM_H_
#define INC_SYSTEM_SYSTEM_H_

#include <stdint.h>

namespace System
{

extern uint32_t system_core_clock; /*!< Core clock */
extern uint32_t ahb_clock;         /*!< AHB clock */
extern uint32_t semc_clock;        /*!< SEMC clock */
extern uint32_t ipg_clock;         /*!< IPG clock */

extern uint32_t osc_clock; /*!< OSC clock selected by PMU_LOWPWR_CTRL[OSC_SEL]. */
extern uint32_t rtc_clock; /*!< RTC clock. (RTCCLK) */

extern uint32_t arm_pll_clock; /*!< ARMPLLCLK. */

extern uint32_t usb1_pll_clock;      /*!< USB1PLLCLK. */
extern uint32_t usb1_pll_pfd0_clock; /*!< USB1PLLPDF0CLK. */
extern uint32_t usb1_pll_pfd1_clock; /*!< USB1PLLPFD1CLK. */
extern uint32_t usb1_pll_pfd2_clock; /*!< USB1PLLPFD2CLK. */
extern uint32_t usb1_pll_pfd3_clock; /*!< USB1PLLPFD3CLK. */

extern uint32_t usb2_pll_clock; /*!< USB2PLLCLK. */

extern uint32_t sys_pll_clock;      /*!< SYSPLLCLK. */
extern uint32_t sys_pll_pfd0_clock; /*!< SYSPLLPDF0CLK. */
extern uint32_t sys_pll_pfd1_clock; /*!< SYSPLLPFD1CLK. */
extern uint32_t sys_pll_pfd2_clock; /*!< SYSPLLPFD2CLK. */
extern uint32_t sys_pll_pfd3_clock; /*!< SYSPLLPFD3CLK. */

extern uint32_t enet_pll0_clock; /*!< Enet PLLCLK ref_enetpll0. */
extern uint32_t enet_pll1_clock; /*!< Enet PLLCLK ref_enetpll1. */

extern uint32_t audio_pll_clock; /*!< Audio PLLCLK. */
extern uint32_t video_pll_clock; /*!< Video PLLCLK. */

void InitSystem();
} // namespace System

#endif /* INC_SYSTEM_SYSTEM_H_ */
