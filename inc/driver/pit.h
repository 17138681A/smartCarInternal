/*
 * pit.h
 *
 *  Created on: Oct 5, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_PIT_H_
#define INC_DRIVER_PIT_H_

#include <functional>
#include "system/cmsis/access_layer/access_layer.h"

namespace Driver
{

/*!
 * @addtogroup pit Periodic Interrupt Timer
 * @ingroup timer
 * The PIT module is an array of timers that can be used to raise interrupts and trigger DMA channels.
 * @{
 */
/**
 * @brief Periodic Interrupt Timer
 */
class Pit
{
  public:
	typedef std::function<void(Pit *)> PIT_Listener;
	struct Config
	{
		enum struct Channel
		{
			k0,
			k1,
			k2,
			k3
		};
		bool start = false;
		// uint32_t period_count;
		uint32_t period_count;
		Channel channel;
		PIT_Listener listener = nullptr;
		uint8_t interrupt_priority = 15; //Interrupt priority, range [0-15], smaller value means higher priority
		bool enable_debug_run = false;
	};
	Pit(const Config &config);

	inline void Start()
	{
		pit_base->CHANNEL[(uint8_t)channel].TCTRL |= PIT_TCTRL_TEN_MASK;
	}
	
	inline void Stop()
	{
		pit_base->CHANNEL[(uint8_t)channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	}

	void ChangePeriod(uint32_t us);	//1000us = 1ms

	inline void ChangePeriodCount(uint32_t count)
	{
		pit_base->CHANNEL[(uint8_t)channel].LDVAL = count;
	}

	inline PIT_Listener GetListener()
	{
		return listener;
	}

  private:
	PIT_Listener listener;
	PIT_Type *pit_base;
	Config::Channel channel;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_PIT_H_ */
