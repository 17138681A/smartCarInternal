/*
 * lcd_console.h
 *
 *  Created on: Nov 8, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_LCD_CONSOLE_H_
#define INC_DEVICE_DRIVER_LCD_CONSOLE_H_

#include "device_driver/lcd.h"

namespace DeviceDriver
{

/*!
 * @ingroup lcd
 * @{
 */
/**
 * @brief lcd console library
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class Console
{
  public:
	Console(Lcd *lcd_ptr);
	void SetWindow(const Lcd::Rect rect);
	void SetTextColor(const uint16_t color);
	void SetTextColor(const uint32_t color);
	void SetCursor(const uint16_t column, const uint16_t row);

	enum FontSize
	{
		k16 = 16,
		k48 = 48
	};

	void WriteString(const char *string, const uint16_t size);
	void WriteValue(const int32_t value, const uint16_t size);
	void WriteValue(const float value, const uint16_t size);

	uint16_t GetColumn() const
	{
		return column;
	}
	uint16_t GetRow() const
	{
		return row;
	}
	const Lcd::Rect GetActiveWindow() const
	{
		return active_window;
	}

  private:
	void DrawChar(const char *character, const uint16_t size);

	Lcd::Rect active_window;
	uint16_t column, row;
	uint32_t text_color;

	Lcd *lcd_ptr;
};
/** @}*/

} // namespace DeviceDriver

#endif /* INC_DEVICE_DRIVER_LCD_CONSOLE_H_ */
