/*
 * st7735r.cpp
 *
 *  Created on: Sep 23, 2018
 *      Author: LeeChunHei
 */

#include "../config/config.h"
#include "device_driver/st7735r.h"
#include "system/pinout/pinout.h"
#include "system/systick.h"
#include <cstdlib>

#ifdef ST7735R_USED

namespace DeviceDriver
{

#define ST7735R_NOP 0x00	 // NOP
#define ST7735R_SWRESET 0x01 // Software Reset
#define ST7735R_RDDID 0x04   // Read Display ID
#define ST7735R_SLPIN 0x10   // Sleep In
#define ST7735R_SLPOUT 0x11  // Sleep Out
#define ST7735R_INVOFF 0x20  // Display Inversion Off
#define ST7735R_INVON 0x21   // Display Inversion On
#define ST7735R_DISPOFF 0x28 // Display Off
#define ST7735R_DISPON 0x29  // Display On
#define ST7735R_CASET 0x2A   // Column Address Set
#define ST7735R_RASET 0x2B   // Row Address Set
#define ST7735R_RAMWR 0x2C   // Memory Write
#define ST7735R_MADCTL 0x36  // Memory Data Access Control
#define ST7735R_COLMOD 0x3A  // Interface Pixel Format
#define ST7735R_FRMCTR1 0xB1 // Frame Rate Control (in normal mode)
#define ST7735R_INVCTR 0xB4  // Display Inversion Control
#define ST7735R_PWCTR1 0xC0  // Power Control 1
#define ST7735R_PWCTR2 0xC1  // Power Control 2
#define ST7735R_PWCTR3 0xC2  // Power Control 3 (in normal mode)
#define ST7735R_PWCTR4 0xC3  // Power Control 4 (in idle/8-bit mode)
#define ST7735R_PWCTR5 0xC4  // Power Control 5 (in partial mode)
#define ST7735R_VMCTR1 0xC5  // VCOM Control 1
#define ST7735R_GMCTRP1 0xE0 // Gamma (+ polarity) Correction Characteristics Setting
#define ST7735R_GMCTRN1 0xE1 // Gamma (- polarity) Correction Characteristics Setting

System::Pinout::Name st7735r_pins[ST7735R_USED * 6] = ST7735R_PINS;

template <typename T>
inline T Clamp(const T &min, const T &x, const T &max)
{
	return x > min ? (x < max ? x : max) : min;
}

Driver::SpiMaster::Config St7735r::GetSpiConfig(System::Pinout::Name scl, System::Pinout::Name sda, System::Pinout::Name cs)
{
	Driver::SpiMaster::Config config;
	uint8_t dummy;
	System::Pinout::Config sda_config;
	sda_config.pin = sda;
	if (System::Pinout::GetSpiSdoPinConfig(sda_config, dummy))
	{
		config.pin_config = Driver::SpiMaster::Config::PinConfig::kSdoInSdoOut;
		config.sdo = sda;
	}
	else
	{
		config.pin_config = Driver::SpiMaster::Config::PinConfig::kSdiInSdiOut;
		config.sdi = sda;
	}
	config.sck = scl;
	config.cs = cs;
	// Max freq of ST7735R == 15MHz
	config.baud_rate = 15000000;
	config.bits_per_frame = 8;
	config.clock_polarity_active_low = false;
	config.transfer_LSB_first = false;
	//	config.data_out_config = Driver::SpiMaster::Config::DataOutConfig::kDataOutTristate;
	config.between_transfer_delay_ns = 10;
	config.last_sck_to_pcs_delay_ns = 10;
	config.pcs_to_sck_delay_ns = 10;
	//	config.clock_phase = Driver::SpiMaster::Config::ClockPhase::kSecondEdge;
	return config;
}

Driver::GPIO::Config St7735r::GetGPIOConfig(System::Pinout::Name gpio_pin)
{
	Driver::GPIO::Config config;
	config.pin = gpio_pin;
	config.default_high = true;
	config.gpio_dir = Driver::GPIO::Direction::kDigitalOutput;
	return config;
}

St7735r::St7735r(const Config &config) : spi_master(GetSpiConfig(st7735r_pins[config.id * 6], st7735r_pins[config.id * 6 + 1], st7735r_pins[config.id * 6 + 2])), dc(
																																								GetGPIOConfig(st7735r_pins[config.id * 6 + 3])),
								   reset(GetGPIOConfig(st7735r_pins[config.id * 6 + 4])), backlight(
																							  GetGPIOConfig(st7735r_pins[config.id * 6 + 5])),
								   lcd_width(
									   (config.orientation == 0 || config.orientation == 2) ? 130 : 161),
								   lcd_height(
									   (config.orientation == 0 || config.orientation == 2) ? 161 : 130),
								   lcd_width_shift(
									   (config.orientation == 0 || config.orientation == 2) ? 2 : 1),
								   lcd_height_shift(
									   (config.orientation == 0 || config.orientation == 2) ? 1 : 2)
{
	region = Rect(0, 0, GetLcdWidth(), GetLcdHeight());
	Clear();
	Send(false, ST7735R_SWRESET);
	System::Systick::DelayMS(10);

	Send(false, ST7735R_SLPOUT);
	System::Systick::DelayMS(120);

	InitMadctl(config);

	// 16-bit
	Send(false, ST7735R_COLMOD);
	Send(true, 0x05);

	InitFrmctr(config);
	InitPwctr();
	InitGamma();

	Send(false, ST7735R_VMCTR1);
	Send(true, 0x0E);

	SetActiveRect();

	Send(false, ST7735R_DISPON);
	System::Systick::DelayMS(10);
}

void St7735r::FillColor(const uint32_t color)
{
	if (region.x + lcd_width_shift >= lcd_width || region.y + lcd_height_shift >= lcd_height)
	{
		return;
	}

	SetActiveRect();
	Send(false, ST7735R_RAMWR);
	const uint32_t max = (Clamp<uint8_t>(0, region.width, lcd_width - region.x + lcd_width_shift) * Clamp<uint8_t>(0, region.height, lcd_height - region.y + lcd_height_shift));
	for (uint32_t i = 0; i < max; ++i)
	{
		Send(true, color >> 8);
		Send(true, color);
	}
}

void St7735r::FillColor(const Rect &rect, const uint32_t color)
{
	region = rect;
	if (region.x + lcd_width_shift >= lcd_width || region.y + lcd_height_shift >= lcd_height)
	{
		return;
	}
	SetActiveRect();
	Send(false, ST7735R_RAMWR);
	const uint32_t max = (Clamp<uint8_t>(0, region.width, lcd_width - region.x + lcd_width_shift) * Clamp<uint8_t>(0, region.height, lcd_height - region.y + lcd_height_shift));
	for (uint32_t i = 0; i < max; ++i)
	{
		Send(true, color >> 8);
		Send(true, color);
	}
}

void St7735r::FillGrayscalePixel(const uint8_t *pixel, const uint32_t length)
{
	if (region.x + lcd_width_shift >= lcd_width || region.y + lcd_height_shift >= lcd_height)
	{
		return;
	}

	SetActiveRect();
	Send(false, ST7735R_RAMWR);
	const uint8_t w = Clamp<uint8_t>(0, region.width, lcd_width - region.x + lcd_width_shift);
	//const Uint h = Clamp<Uint>(0, region.h, lcd_height - region.y+lcd_height_shift);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const uint32_t length_ = std::min(region.width * region.height, length);
	for (uint32_t row_beg = 0; row_beg < length_; row_beg += region.width)
	{
		for (uint32_t x = 0; x < w; ++x)
		{
			const uint8_t gs_color = pixel[row_beg + x];
			const uint16_t color = ((gs_color >> 3) << 11) | ((gs_color >> 2) << 5) | (gs_color >> 3);
			Send(true, color >> 8);
			Send(true, color);
		}
	}
}

void St7735r::FillColorPixel(const uint16_t *pixel, const uint32_t length)
{
	if (region.x + lcd_width_shift >= lcd_width || region.y + lcd_height_shift >= lcd_height)
	{
		return;
	}

	SetActiveRect();
	Send(false, ST7735R_RAMWR);
	const uint8_t w = Clamp<uint8_t>(0, region.width, lcd_width - region.x + lcd_width_shift);
	//const Uint h = Clamp<Uint>(0, region.h, lcd_height - region.y+lcd_height_shift);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const uint32_t length_ = std::min(region.width * region.height, length);
	for (uint32_t row_beg = 0; row_beg < length_; row_beg += region.width)
	{
		for (uint32_t x = 0; x < w; ++x)
		{
			Send(true, pixel[row_beg + x] >> 8);
			Send(true, pixel[row_beg + x]);
		}
	}
}

void St7735r::FillColorPixel(const uint32_t *pixel, const uint32_t length)
{
	if (region.x + lcd_width_shift >= lcd_width || region.y + lcd_height_shift >= lcd_height)
	{
		return;
	}

	SetActiveRect();
	Send(false, ST7735R_RAMWR);
	const uint8_t w = Clamp<uint8_t>(0, region.width, lcd_width - region.x + lcd_width_shift);
	//const Uint h = Clamp<Uint>(0, region.h, lcd_height - region.y+lcd_height_shift);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const uint32_t length_ = std::min(region.width * region.height, length);
	for (uint32_t row_beg = 0; row_beg < length_; row_beg += region.width)
	{
		for (uint32_t x = 0; x < w; ++x)
		{
			uint32_t pixel_show = pixel[row_beg + x];
			pixel_show = (uint16_t)(((pixel_show >> 8) & 0xF800) | ((pixel_show >> 5) & 0x7E) | (pixel_show >> 3));
			Send(true, pixel_show >> 8);
			Send(true, pixel_show);
		}
	}
}

void St7735r::FillBits(const uint32_t color_t, const uint32_t color_f, const uint8_t *data, const uint32_t bit_length)
{
	if (region.x + lcd_width_shift >= lcd_width || region.y + lcd_height_shift >= lcd_height)
	{
		return;
	}

	SetActiveRect();
	Send(false, ST7735R_RAMWR);
	const uint8_t w = Clamp<uint8_t>(0, region.width, lcd_width - region.x + lcd_width_shift);
	//const Uint h = Clamp<Uint>(0, region.h, lcd_height - region.y+lcd_height_shift);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const uint32_t length_ = std::min(region.width * region.height, bit_length);
	uint32_t pos = 0;
	int bit_pos = 8;
	for (uint32_t row_beg = 0; row_beg < length_; row_beg += region.width)
	{
		for (uint32_t x = 0; x < w; ++x)
		{
			if (--bit_pos < 0)
			{
				bit_pos = 7;
				++pos;
			}
			if ((data[pos] >> bit_pos) & 1)
			{
				Send(true, color_t >> 8);
				Send(true, color_t);
			}
			else
			{
				Send(true, color_f >> 8);
				Send(true, color_f);
			}
		}

		bit_pos -= (region.width - w) % 8;
		pos += (region.width - w) >> 3; // /8
	}
}

void St7735r::Clear()
{
	ClearRegion();
	FillColor(0);
}

void St7735r::Clear(const uint32_t color)
{
	ClearRegion();
	FillColor(color);
}

void St7735r::SetInvertColor(const bool flag)
{
	if (flag)
	{
		Send(false, ST7735R_INVON);
	}
	else
	{
		Send(false, ST7735R_INVOFF);
	}
}

void St7735r::InitMadctl(const Config &config)
{
	uint8_t param = 0;
	switch (config.orientation)
	{
	case 1:
		param = 1 << 5 | 1 << 7;
		break;
	case 2:
		param = 1 << 7 | 1 << 6;
		break;
	case 3:
		param = 1 << 5 | 1 << 6;
		break;
	default:
		break;
	}
	param |= config.is_bgr << 4;

	Send(false, ST7735R_MADCTL);
	Send(true, param);
}
void St7735r::InitFrmctr(const Config &config)
{
	const uint8_t line = 160;
	const uint32_t fosc = 850000;
	uint32_t best_rtna = 0;
	uint32_t best_fpa = 0;
	uint32_t best_bpa = 0;
	uint32_t min_diff = static_cast<uint32_t>(-1);
	for (uint32_t rtna = 0; rtna <= 0x0F; ++rtna)
	{
		const uint32_t this_rtna = rtna * 2 + 40;
		for (uint32_t fpa = 1; fpa <= 0x3F; ++fpa)
		{
			for (uint32_t bpa = 1; bpa <= 0x3F; ++bpa)
			{
				const uint32_t this_rate = fosc / (this_rtna * (line + fpa + bpa + 2));
				const uint32_t this_diff = std::abs((int32_t)(this_rate - config.fps));
				if (this_diff < min_diff)
				{
					min_diff = this_diff;
					best_rtna = rtna;
					best_fpa = fpa;
					best_bpa = bpa;
				}

				if (min_diff == 0)
				{
					break;
				}
			}
		}
	}

	Send(false, ST7735R_FRMCTR1);
	Send(true, best_rtna);
	Send(true, best_fpa);
	Send(true, best_bpa);
}
void St7735r::InitPwctr()
{
	Send(false, ST7735R_PWCTR1);
	Send(true, 0xA2);
	Send(true, 0x02);
	Send(true, 0x84);

	Send(false, ST7735R_PWCTR2);
	Send(true, 0xC5);

	Send(false, ST7735R_PWCTR3);
	Send(true, 0x0A);
	Send(true, 0x00);

	Send(false, ST7735R_PWCTR4);
	Send(true, 0x8A);
	Send(true, 0x2A);

	Send(false, ST7735R_PWCTR5);
	Send(true, 0x8A);
	Send(true, 0xEE);
}
void St7735r::InitGamma()
{
	Send(false, ST7735R_GMCTRP1);
	Send(true, 0x02);
	Send(true, 0x1C);
	Send(true, 0x07);
	Send(true, 0x12);
	Send(true, 0x37);
	Send(true, 0x32);
	Send(true, 0x29);
	Send(true, 0x2D);
	Send(true, 0x29);
	Send(true, 0x25);
	Send(true, 0x2B);
	Send(true, 0x39);
	Send(true, 0x00);
	Send(true, 0x01);
	Send(true, 0x03);
	Send(true, 0x10);

	Send(false, ST7735R_GMCTRN1);
	Send(true, 0x03);
	Send(true, 0x1D);
	Send(true, 0x07);
	Send(true, 0x06);
	Send(true, 0x2E);
	Send(true, 0x2C);
	Send(true, 0x29);
	Send(true, 0x2D);
	Send(true, 0x2E);
	Send(true, 0x2E);
	Send(true, 0x37);
	Send(true, 0x3F);
	Send(true, 0x00);
	Send(true, 0x00);
	Send(true, 0x02);
	Send(true, 0x10);
}
void St7735r::SetActiveRect()
{
	Send(false, ST7735R_CASET);
	// start
	Send(true, 0x00);
	Send(true, region.x + lcd_width_shift);
	// end
	Send(true, 0x00);
	Send(true, region.x + lcd_width_shift + region.width - 1);

	Send(false, ST7735R_RASET);
	Send(true, 0x00);
	Send(true, region.y + lcd_height_shift);
	Send(true, 0x00);
	Send(true, region.y + lcd_height_shift + region.height - 1);
}

inline void St7735r::Send(const bool is_data, const uint8_t data)
{
	while (spi_master.GetTxFifoCount());
	dc.Set(is_data);
	spi_master.SendData(data);
	while (!spi_master.GetTransferState())
		;
	spi_master.ClearTransferState();
}

} // namespace DeviceDriver

#endif
