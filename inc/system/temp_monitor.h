/*
 * temp_monitor.h
 *
 *  Created on: 23 Mar 2019
 *      Author: LeeChunHei
 */

#ifndef DRIVER_TEMP_MONITOR_H_
#define DRIVER_TEMP_MONITOR_H_

#include "system/cmsis/access_layer/access_layer.h"

namespace System
{

class TempMonitor
{
  public:
	static void Init();
	static float GetTemperature();
};

} // namespace System

#endif /* DRIVER_TEMP_MONITOR_H_ */
