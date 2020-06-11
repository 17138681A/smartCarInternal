/*
 * firege_5inch_touchscreen.h
 *
 *  Created on: Oct 27, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_FIREGE_5INCH_TOUCHSCREEN_H_
#define INC_DEVICE_DRIVER_FIREGE_5INCH_TOUCHSCREEN_H_

#include "driver/elcdif.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "device_driver/touchscreen_lcd.h"
#include <vector>

namespace DeviceDriver
{

/*!
 * @ingroup lcd
 * @{
 */
/**
 * @brief Firege 5-inch touchscreen driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class Firege5InchTouchScreen : public TouchScreenLcd
{
  public:
	struct Config
	{
		uint8_t id;
		uint8_t fps;
		// Orientation of the screen (four direction: 0 1 2 3, count in clockwise)
		uint8_t orientation = 0;
		Driver::I2CMaster *i2c_master = nullptr;
	};
	Firege5InchTouchScreen(const Config &config);
	uint16_t GetLcdWidth()
	{
		return width;
	}
	uint16_t GetLcdHeight()
	{
		return height;
	}
	void SetRegion(const Rect &rect)
	{
		region = rect;
	}
	Rect GetRegion()
	{
		return region;
	}
	void ClearRegion()
	{
		region = Rect{0, 0, GetLcdWidth(), GetLcdHeight()};
	}
	uint32_t *LockFrameBuffer();
	void UnlockFrameBuffer()
	{
		buffer_writing = false;
	}
	void FillColor(const uint32_t color) override;
	void FillColor(const Rect &rect, const uint32_t color) override;
	void FillGrayscalePixel(const uint8_t *pixel, const uint32_t length) override;
	void FillColorPixel(const uint16_t *pixel, const uint32_t length) override;
	void FillColorPixel(const uint32_t *pixel, const uint32_t length) override;
	void FillBits(const uint32_t color_t, const uint32_t color_f, const uint8_t *data, const uint32_t bit_length) override;
	void Clear() override;
	void Clear(const uint32_t color) override;
	uint32_t GetTouchLocation(int32_t *x, int32_t *y) override;
	uint32_t GetActiveTouchNum() override;
	Gesture GetGesture(uint32_t &param_1, uint32_t &param2) override;
	inline Driver::I2CMaster *GetI2CMaster()
	{
		return i2c_master;
	}

  private:
	void BufferCopy();

	Driver::eLCDIF elcdif;
	Driver::I2CMaster *i2c_master;
	Driver::GPIO touch_int, touch_rst, bk, disp;

	uint16_t width, height;
	bool buffer_swapped, buffer_writing, buffer_changed;
	uint8_t active_buffer, orientation;
	Lcd::Rect region;

	uint8_t touching_num;
	std::vector<uint8_t> touching_id;
	std::vector<int32_t> touch_x_coor;
	std::vector<int32_t> touch_y_coor;

	Gesture point_gesture[5];
	int32_t gesture_x_coor[5];
	int32_t gesture_y_coor[5];
};
/** @}*/

} // namespace DeviceDriver

#endif /* INC_DEVICE_DRIVER_FIREGE_5INCH_TOUCHSCREEN_H_ */
