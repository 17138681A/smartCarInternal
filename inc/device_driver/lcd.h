/*
 * lcd.h
 *
 *  Created on: Sep 23, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_LCD_H_
#define INC_DEVICE_DRIVER_LCD_H_

#include <stdint.h>

namespace DeviceDriver
{

/*!
 * @addtogroup lcd LCD
 * @ingroup device_driver
 * @{
 */
/**
 * @brief Protocol of lcd driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class Lcd
{
  public:
	struct Rect
	{
		uint32_t x;
		uint32_t y;
		uint32_t width;
		uint32_t height;
		Rect() = default;
		Rect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) : x(x), y(y), width(width), height(height)
		{
		}
	};

	virtual ~Lcd()
	{
	}

	enum Color
	{
		kBlack = 0x0000,
		kGray = 0x7BEF,
		kWhite = 0xFFFF,
		kRed = 0xF800,
		kYellow = 0xFFE0,
		kGreen = 0x07E0,
		kCyan = 0x07FF,
		kBlue = 0x001F,
		kPurple = 0xF81F,
	};

	virtual uint16_t GetLcdWidth() = 0;
	virtual uint16_t GetLcdHeight() = 0;
	virtual void SetRegion(const Rect &rect) = 0;
	virtual Rect GetRegion() = 0;
	virtual void ClearRegion() = 0;
	virtual void FillColor(const uint32_t color) = 0;
	virtual void FillColor(const Rect &rect, const uint32_t color) = 0;
	virtual void FillGrayscalePixel(const uint8_t *pixel, const uint32_t length) = 0;
	virtual void FillColorPixel(const uint16_t *pixel, const uint32_t length) = 0;
	virtual void FillColorPixel(const uint32_t *pixel, const uint32_t length) = 0;
	virtual void FillBits(const uint32_t color_t, const uint32_t color_f, const uint8_t *data, const uint32_t bit_length) = 0;
	void DrawLine(const uint32_t start_x, const uint32_t start_y, const uint32_t end_x, const uint32_t end_y, const uint32_t color, const uint32_t width = 1);
	void DrawCircle(const uint32_t x, const uint32_t y, const uint32_t radius, const uint32_t boundary_color, const uint32_t width = 1, const bool color_filled = false, const uint32_t fill_color = 0);
	virtual void Clear() = 0;
	virtual void Clear(const uint32_t color) = 0;
};
/** @}*/

} // namespace DeviceDriver

#endif /* INC_DEVICE_DRIVER_LCD_H_ */
