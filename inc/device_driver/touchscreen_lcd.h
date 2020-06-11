/*
 * touchscreen_lcd.h
 *
 *  Created on: Oct 31, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_TOUCHSCREEN_LCD_H_
#define INC_DEVICE_DRIVER_TOUCHSCREEN_LCD_H_

#include "device_driver/lcd.h"

namespace DeviceDriver
{

/*!
 * @ingroup lcd
 * @{
 */
/**
 * @brief protocol of touchscreen lcd
 */
class TouchScreenLcd : public Lcd
{
  public:
	enum struct Gesture
	{
		kDisable,
		kTap,
		kDrag,
		kSwipeUp,
		kSwipeDown,
		kSwipeLeft,
		kSwipeRight,
		kZoomIn,
		kZoomOut,
		kRotate
	};

	virtual ~TouchScreenLcd()
	{
	}
	virtual uint32_t GetTouchLocation(int32_t *x, int32_t *y) = 0;
	virtual uint32_t GetActiveTouchNum() = 0;
	virtual Gesture GetGesture(uint32_t &param1, uint32_t &param2) = 0;

  private:
	uint32_t touch_x1 = 0;
	uint32_t touch_y1 = 0;
	uint32_t touch_x2 = 0;
	uint32_t touch_y2 = 0;
};
/** @}*/

} // namespace DeviceDriver

#endif /* INC_DEVICE_DRIVER_TOUCHSCREEN_LCD_H_ */
