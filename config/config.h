/*
 * config.h
 *
 *  Created on: Aug 3, 2018
 *      Author: LeeChunHei
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#if defined(FOUNDER)
#include "founder.h"
#elif defined(MINI)
#include "mini.h"
#elif defined(k2019_BEACON_1)
#include "2019_beacon_1.h"
#elif defined(k2019_BEACON_2)
#include "2019_beacon_2.h"
#elif defined(k2019_BEACON_2B)
#include "2019_beacon_2b.h"
#elif defined(k2019_CREATIVE_2)
#include "2019_creative_2.h"
#elif defined(k2019_CREATIVE_2_CM)
#include "2019_creative_2_cm.h"
#elif defined(k2019_CAMERA_1)
#include "2019_camera_1.h"
#elif defined(k2019_CAMERA_2)
#include "2019_camera_2.h"
#elif defined(k2019_CAMERA_3)
#include "2019_camera_3.h"
#elif defined(k2019_BALANCE_1)
#include "2019_balance_1.h"
#elif defined(k2019_BALANCE_2)
#include "2019_balance_2.h"
#else
#error Unknown board

#endif


#endif /* CONFIG_CONFIG_H_ */
