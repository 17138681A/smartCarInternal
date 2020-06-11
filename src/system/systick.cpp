/*
 * systick.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: LeeChunHei
 */

#include "system/systick.h"

namespace System
{

volatile uint32_t Systick::tick_interrupt_count = 0;

void Systick::Init()
{
	tick_interrupt_count = 0;
	SysTick->LOAD = (uint32_t)(SysTick_LOAD_RELOAD_Msk);
	NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);								 /* set Priority for Systick Interrupt */
	SysTick->VAL = 0UL;																				 /* Load the SysTick Counter Value */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
}

#if defined(__cplusplus)
extern "C"
{
#endif
	void SysTick_Handler()
	{
		Systick::tick_interrupt_count++;
	}
#if defined(__cplusplus)
}
#endif

} // namespace System
