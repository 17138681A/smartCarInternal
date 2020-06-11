/*
 * temp_monitor.cpp
 *
 *  Created on: 23 Mar 2019
 *      Author: LeeChunHei
 */

#include "system/temp_monitor.h"

namespace System
{

void TempMonitor::Init()
{
	TEMPMON->TEMPSENSE0_CLR |= TEMPMON_TEMPSENSE0_POWER_DOWN_MASK;
	TEMPMON->TEMPSENSE1 = TEMPMON_TEMPSENSE1_MEASURE_FREQ(1);
	TEMPMON->TEMPSENSE0 |= TEMPMON_TEMPSENSE0_ALARM_VALUE_MASK | TEMPMON_TEMPSENSE0_MEASURE_TEMP_MASK;
}

float TempMonitor::GetTemperature()
{
	while (!(TEMPMON->TEMPSENSE0 & TEMPMON_TEMPSENSE0_CLR_FINISHED_MASK))
		;
	uint16_t raw = (TEMPMON->TEMPSENSE0 & TEMPMON_TEMPSENSE0_TEMP_CNT_MASK) >> TEMPMON_TEMPSENSE0_TEMP_CNT_SHIFT;
	uint8_t hot_temp = OCOTP->ANA1 & 0xFF;
	uint16_t hot_count = (OCOTP->ANA1 >> 8) & 0xFFF;
	uint16_t room_count = (OCOTP->ANA1 >> 20) & 0xFFF;
	return hot_temp - (raw - hot_count) * ((hot_temp - 25.0) / (room_count - hot_count));
}

} // namespace System
