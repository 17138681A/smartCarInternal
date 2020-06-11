/*
 * timer.h
 *
 *  Created on: 28 Jan 2019
 *      Author: lch19
 */

#ifndef DRIVER_TIMER_H_
#define DRIVER_TIMER_H_

#include "system/pinout/pinout.h"

namespace Driver
{

/*!
 * @addtogroup timer Timer
 * @ingroup driver
 * @{
 */
/**
 * @brief Protocol for timer library
 */
class Timer
{
  public:
	virtual ~Timer()
	{
	}
	virtual void SetDutyCycle(uint16_t duty_cycle) = 0;
};
/** @}*/

} // namespace Driver

#endif /* DRIVER_TIMER_H_ */
