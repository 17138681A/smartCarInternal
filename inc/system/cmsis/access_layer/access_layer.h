/*
 * access_layer.h
 *
 *  Created on: Aug 3, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_CMSIS_ACCESS_LAYER_H_
#define INC_CMSIS_ACCESS_LAYER_H_

#if defined(RT1052)
#include "system/cmsis/access_layer/MIMXRT1052.h"
#define ARM_MATH_CM7

#else
#error Unknown MCU

#endif

#endif /* INC_CMSIS_ACCESS_LAYER_H_ */
