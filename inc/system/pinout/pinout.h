/*
 * pinout.h
 *
 *  Created on: Aug 26, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_SYSTEM_PINOUT_PINOUT_H_
#define INC_SYSTEM_PINOUT_PINOUT_H_

#if defined(RT1052)
#include "system/pinout/MIMXRT1052.h"

#else
#error Unknown MCU

#endif

#endif /* INC_SYSTEM_PINOUT_PINOUT_H_ */
