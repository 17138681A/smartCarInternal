/*
 * st7735r.h
 *
 *  Created on: Sep 23, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_ST7735R_H_
#define INC_DEVICE_DRIVER_ST7735R_H_

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "device_driver/lcd.h"
#include "../config/config.h"

#ifdef ST7735R_USED

namespace DeviceDriver
{

/*!
 * @ingroup lcd
 * @{
 */
/**
 * @brief St7735r lcd driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class St7735r : public Lcd
{
  public:
	struct Config
	{
		uint8_t id;
		// Orientation of the screen (four direction: 0 1 2 3, count in clockwise)
		uint8_t orientation = 0;
		/// Whether using a BGR panel instead of a RGB one
		bool is_bgr = false;
		/// Frame rate of the screen
		uint8_t fps = 60;
	};
	St7735r(const Config &config);
	uint16_t GetLcdWidth() override
	{
		return lcd_width - lcd_width_shift;
	}
	uint16_t GetLcdHeight() override
	{
		return lcd_height - lcd_height_shift;
	}
	void SetRegion(const Rect &rect) override
	{
		region = rect;
	}
	Rect GetRegion() override
	{
		return region;
	}

	void ClearRegion() override
	{
		region = Rect{0, 0, GetLcdWidth(), GetLcdHeight()};
	}

	void FillColor(const uint32_t color) override;
	void FillColor(const Rect &rect, const uint32_t color) override;
	void FillGrayscalePixel(const uint8_t *pixel, const uint32_t length) override;
	void FillColorPixel(const uint16_t *pixel, const uint32_t length) override;
	void FillColorPixel(const uint32_t *pixel, const uint32_t length) override;
	void FillBits(const uint32_t color_t, const uint32_t color_f, const uint8_t *data, const uint32_t bit_length) override;

	void Clear() override;
	void Clear(const uint32_t color) override;
	void SetInvertColor(const bool flag);

  private:
	static Driver::SpiMaster::Config GetSpiConfig(System::Pinout::Name scl, System::Pinout::Name sda, System::Pinout::Name cs);
	static Driver::GPIO::Config GetGPIOConfig(System::Pinout::Name gpio_pin);

	void InitMadctl(const Config &config);
	void InitFrmctr(const Config &config);
	void InitPwctr();
	void InitGamma();
	void SetActiveRect();
	inline void Send(const bool is_cmd, const uint8_t data);

	Driver::SpiMaster spi_master;
	Driver::GPIO dc, reset, backlight;
	const uint8_t lcd_width, lcd_height, lcd_width_shift, lcd_height_shift;
	Rect region;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* INC_DEVICE_DRIVER_ST7735R_H_ */
