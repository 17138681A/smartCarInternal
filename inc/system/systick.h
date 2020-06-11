/*
 * systick.h
 *
 *  Created on: Aug 22, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_SYSTICK_H_
#define INC_DRIVER_SYSTICK_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/system.h"
#include "../config/config.h"

namespace System
{

class Systick
{
  public:
	static void Init();
	static inline uint32_t GetTimeInS()
	{
		return (uint32_t)((float)(SysTick_LOAD_RELOAD_Msk - SysTick->VAL) / system_core_clock + (float)tick_interrupt_count / system_core_clock * SysTick_LOAD_RELOAD_Msk);
	}
	static inline uint32_t GetTimeInMS()
	{
		return (uint32_t)((float)(SysTick_LOAD_RELOAD_Msk - SysTick->VAL) / system_core_clock * 1000 + (float)tick_interrupt_count / system_core_clock * SysTick_LOAD_RELOAD_Msk * 1000);
	}
	static inline uint32_t GetTimeInUS()
	{
		return (uint32_t)((float)(SysTick_LOAD_RELOAD_Msk - SysTick->VAL) / system_core_clock * 1000000 + (float)tick_interrupt_count / system_core_clock * SysTick_LOAD_RELOAD_Msk * 1000000); //((SysTick_LOAD_RELOAD_Msk - SysTick->VAL + tick_interrupt_count * SysTick_LOAD_RELOAD_Msk) / system_core_clock * 1000000);
	}
	static inline uint32_t GetTickCount()
	{
		return (SysTick_LOAD_RELOAD_Msk - SysTick->VAL + tick_interrupt_count * SysTick_LOAD_RELOAD_Msk);
	}
	static inline void DelayS(uint32_t second)
	{
		while (second--)
		{
			DelayMS(1000);
		}
	}
	static inline void DelayMS(uint32_t milisecond)
	{
		while (milisecond--)
		{
			DelayUS(1000);
		}
	}
	static inline void DelayUS(uint32_t microsecond)
	{
		uint32_t load_val = (uint32_t)(1000 * (system_core_clock / 1000000000.0));
		while (microsecond--)
		{
			DelayCount(load_val);
		}
	}
	static inline void DelayNS(uint32_t nanosecond)
	{
		SysTick->CTRL = 0x00;
		volatile uint32_t temp = SysTick->VAL;
		volatile uint32_t nano_tick = (uint32_t)(nanosecond * (system_core_clock / 1000000000.0));
		if(temp<=nano_tick){
			++tick_interrupt_count;
			temp = SysTick_LOAD_RELOAD_Msk - (nano_tick-temp);
		}else
		{
			temp-=nano_tick;
		}
		SysTick->LOAD = nano_tick;
		SysTick->VAL = 0x00;
		SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
			;
		SysTick->CTRL = 0x00;
		SysTick->LOAD = temp;
		SysTick->VAL = 0x0;
		SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk); //Enable back systick interrupt
		SysTick->LOAD = (uint32_t)(SysTick_LOAD_RELOAD_Msk);
	}
	static inline void DelayCount(uint32_t count)
	{
		SysTick->CTRL = 0x00;
		volatile uint32_t temp = SysTick->VAL;
		if(temp<=count){
			++tick_interrupt_count;
			temp = SysTick_LOAD_RELOAD_Msk - (count-temp);
		}else{
			temp-=count;
		}
		SysTick->LOAD = count;
		SysTick->VAL = 0x00;
		SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
			;
		SysTick->CTRL = 0x00;
		SysTick->LOAD = temp;
		SysTick->VAL = 0x0;
		SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk); //Enable back systick interrupt
		SysTick->LOAD = (uint32_t)(SysTick_LOAD_RELOAD_Msk);
	}
	static volatile uint32_t tick_interrupt_count;
};

} // namespace System

#endif /* INC_DRIVER_SYSTICK_H_ */
