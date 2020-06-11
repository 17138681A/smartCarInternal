/*
 * firege_5inch_touchscreen.cpp
 *
 *  Created on: Oct 27, 2018
 *      Author: LeeChunHei
 */

#include "device_driver/firege_5inch_touchscreen.h"
#include "driver/hard_i2c_master.h"
#include "driver/soft_i2c_master.h"
#include "system/systick.h"
#include "../config/config.h"
#include <cstring>
#include <cmath>

namespace DeviceDriver
{

#define GTP_ADDRESS 0x5D
#define GTP_REG_VERSION 0x8140
#define GTP_ADDR_LENGTH 2
#define GTP_CONFIG_MAX_LENGTH 240
#define GTP_REG_CONFIG_DATA 0x8047
#define GTP_READ_COOR_ADDR 0x814E

__attribute__((section("NonCacheable,\"aw\",%nobits @"))) static uint32_t lcd_buffer[2][384000] __attribute__((aligned(64)));

const uint8_t CTP_CFG_GT9157[] = {0x00, 0x20, 0x03, 0xE0, 0x01, 0x05, 0x3C, 0x00, 0x01, 0x08, 0x28, 0x0C, 0x50, 0x32, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x19, 0x1E, 0x14, 0x8B, 0x2B, 0x0D, 0x33, 0x35, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x9A, 0x03, 0x11, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x20, 0x58, 0x94, 0xC5, 0x02, 0x00, 0x00, 0x00, 0x04, 0xB0, 0x23, 0x00, 0x93, 0x2B, 0x00, 0x7B, 0x35, 0x00, 0x69, 0x41, 0x00, 0x5B, 0x4F, 0x00, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0F, 0x10, 0x12, 0x13, 0x16, 0x18, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x24, 0x26, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x48, 0x01};

Driver::eLCDIF::Config GetFirege5InchTouchScreeneLCDIFConfig(const Firege5InchTouchScreen::Config &config)
{
	Driver::eLCDIF::Config lcd_config;
	lcd_config.frame_rate = config.fps;
	lcd_config.pin_list.data0 = FIREGE5INCHTOUCHSCREEN_D0(config.id);
	lcd_config.pin_list.data1 = FIREGE5INCHTOUCHSCREEN_D1(config.id);
	lcd_config.pin_list.data2 = FIREGE5INCHTOUCHSCREEN_D2(config.id);
	lcd_config.pin_list.data3 = FIREGE5INCHTOUCHSCREEN_D3(config.id);
	lcd_config.pin_list.data4 = FIREGE5INCHTOUCHSCREEN_D4(config.id);
	lcd_config.pin_list.data5 = FIREGE5INCHTOUCHSCREEN_D5(config.id);
	lcd_config.pin_list.data6 = FIREGE5INCHTOUCHSCREEN_D6(config.id);
	lcd_config.pin_list.data7 = FIREGE5INCHTOUCHSCREEN_D7(config.id);
	lcd_config.pin_list.data8 = FIREGE5INCHTOUCHSCREEN_D8(config.id);
	lcd_config.pin_list.data9 = FIREGE5INCHTOUCHSCREEN_D9(config.id);
	lcd_config.pin_list.data10 = FIREGE5INCHTOUCHSCREEN_D10(config.id);
	lcd_config.pin_list.data11 = FIREGE5INCHTOUCHSCREEN_D11(config.id);
	lcd_config.pin_list.data12 = FIREGE5INCHTOUCHSCREEN_D12(config.id);
	lcd_config.pin_list.data13 = FIREGE5INCHTOUCHSCREEN_D13(config.id);
	lcd_config.pin_list.data14 = FIREGE5INCHTOUCHSCREEN_D14(config.id);
	lcd_config.pin_list.data15 = FIREGE5INCHTOUCHSCREEN_D15(config.id);
	lcd_config.pin_list.data16 = FIREGE5INCHTOUCHSCREEN_D16(config.id);
	lcd_config.pin_list.data17 = FIREGE5INCHTOUCHSCREEN_D17(config.id);
	lcd_config.pin_list.data18 = FIREGE5INCHTOUCHSCREEN_D18(config.id);
	lcd_config.pin_list.data19 = FIREGE5INCHTOUCHSCREEN_D19(config.id);
	lcd_config.pin_list.data20 = FIREGE5INCHTOUCHSCREEN_D20(config.id);
	lcd_config.pin_list.data21 = FIREGE5INCHTOUCHSCREEN_D21(config.id);
	lcd_config.pin_list.data22 = FIREGE5INCHTOUCHSCREEN_D22(config.id);
	lcd_config.pin_list.data23 = FIREGE5INCHTOUCHSCREEN_D23(config.id);
	lcd_config.pin_list.pclk = FIREGE5INCHTOUCHSCREEN_PCLK(config.id);
	lcd_config.pin_list.enable = FIREGE5INCHTOUCHSCREEN_ENABLE(config.id);
	lcd_config.pin_list.hsync = FIREGE5INCHTOUCHSCREEN_HSYNC(config.id);
	lcd_config.pin_list.vsync = FIREGE5INCHTOUCHSCREEN_VSYNC(config.id);
	lcd_config.buffer_addr = (uint32_t)(lcd_buffer[1]);
	lcd_config.width = 800;
	lcd_config.height = 480;
	lcd_config.horizontal_back_porch = 46;
	lcd_config.horizontal_front_porch = 22;
	lcd_config.hsync_pulse_width = 1;
	lcd_config.vsync_pulse_width = 1;
	lcd_config.vertical_front_porch = 22;
	lcd_config.vertical_back_porch = 23;
	lcd_config.data_enable_active_high = true;
	lcd_config.vsync_active_high = false;
	lcd_config.hsync_active_high = false;
	lcd_config.drive_data_on_rising_clk_edge = true;
	lcd_config.pixel_format = Driver::eLCDIF::Config::PixelFormat::kXRGB8888;
	lcd_config.data_bus = Driver::eLCDIF::Config::DataBus::k24Bit;

	return lcd_config;
}

Driver::I2CMaster::Config GetFirege5InchTouchScreenI2CConfig(System::Pinout::Name scl, System::Pinout::Name sda)
{
	Driver::I2CMaster::Config config;
	config.scl = scl;
	config.sda = sda;
	config.send_wait_time = 200000;
	config.recieve_wait_time = 200000;
	config.debug_enable = true;
	config.ignore_ack = true;
	config.baud_rate_Hz = 100000;
	return config;
}

Driver::GPIO::Config GetFirege5InchTouchScreenGPIConfig(System::Pinout::Name pin)
{
	Driver::GPIO::Config gpio_config;
	gpio_config.pin = pin;
	gpio_config.gpio_dir = Driver::GPIO::Direction::kDigitalInput;
	System::Pinout::Config::PinConfig pin_config;
	pin_config.open_drain_enable = true;
	pin_config.pull_keep_config = System::Pinout::Config::PinConfig::PullKeepConfig::kPull;
	pin_config.pull_config = System::Pinout::Config::PinConfig::PullConfig::k100kPullDown;
	gpio_config.pin_config = pin_config;
	return gpio_config;
}

Driver::GPIO::Config GetFirege5InchTouchScreenGPOConfig(System::Pinout::Name pin)
{
	Driver::GPIO::Config gpio_config;
	gpio_config.pin = pin;
	gpio_config.gpio_dir = Driver::GPIO::Direction::kDigitalOutput;
	gpio_config.default_high = false;
	System::Pinout::Config::PinConfig pin_config;
	pin_config.drive_strength = System::Pinout::Config::PinConfig::DriveStrength::kDSE7;
	pin_config.fast_slew_rate = true;
	pin_config.hysteresis_enable = false;
	pin_config.open_drain_enable = false;
	pin_config.speed = System::Pinout::Config::PinConfig::Speed::k200MHz;
	pin_config.pull_keep_config = System::Pinout::Config::PinConfig::PullKeepConfig::kKeep;
	gpio_config.pin_config = pin_config;
	return gpio_config;
}

Firege5InchTouchScreen::Firege5InchTouchScreen(const Config &config) : elcdif(GetFirege5InchTouchScreeneLCDIFConfig(config)), touch_int(GetFirege5InchTouchScreenGPIConfig(FIREGE5INCHTOUCHSCREEN_INT(config.id))), touch_rst(GetFirege5InchTouchScreenGPOConfig(FIREGE5INCHTOUCHSCREEN_RST(config.id))), bk(GetFirege5InchTouchScreenGPOConfig(FIREGE5INCHTOUCHSCREEN_BK(config.id))), disp(GetFirege5InchTouchScreenGPOConfig(FIREGE5INCHTOUCHSCREEN_DISP(config.id)))
{
	if (config.i2c_master)
	{
		i2c_master = config.i2c_master;
	}
	else
	{
		System::Pinout::Config scl_config, sda_config;
		uint8_t scl_module, sda_module;
		scl_config.pin = FIREGE5INCHTOUCHSCREEN_SCL(config.id);
		sda_config.pin = FIREGE5INCHTOUCHSCREEN_SDA(config.id);
		Driver::I2CMaster::Config i2c_config = GetFirege5InchTouchScreenI2CConfig(scl_config.pin, sda_config.pin);
		if (!System::Pinout::GetI2CSclPinConfig(scl_config, scl_module) || !System::Pinout::GetI2CSdaPinConfig(sda_config, sda_module) || (scl_module != sda_module))
		{
			i2c_master = new Driver::SoftI2CMaster(i2c_config);
		}
		else
		{
			i2c_master = new Driver::SoftI2CMaster(i2c_config);
		}
	}
	buffer_swapped = false;
	buffer_writing = false;
	buffer_changed = false;
	active_buffer = 0;
	orientation = config.orientation;
	touching_num = 0;
	touching_id.clear();
	touch_x_coor.clear();
	touch_y_coor.clear();
	for (int i = 0; i < 5; i++)
	{
		point_gesture[i] = Gesture::kDisable;
	}
	for (uint8_t i = 0; i < 5; i++)
	{
		gesture_x_coor[i] = -1;
		gesture_y_coor[i] = -1;
	}
	if (orientation % 2)
	{
		width = 800;
		height = 480;
	}
	else
	{
		width = 480;
		height = 800;
	}
	region = Rect(0, 0, 800, 480);
	elcdif.SetListener([&](Driver::eLCDIF *elcdif_ptr) {
		if (!buffer_writing)
		{
			if (buffer_changed)
			{
				elcdif_ptr->SetCurrBufferAddr((uint32_t)(lcd_buffer[active_buffer]));
				elcdif_ptr->SetNextBufferAddr((uint32_t)(lcd_buffer[active_buffer]));
				active_buffer ^= 1;
				buffer_changed = false;
				buffer_swapped = true;
			}
		}
	},
					   10);
	for (uint32_t i = 0; i < 384000; i++)
	{
		lcd_buffer[0][i] = 16777216;
		lcd_buffer[1][i] = 16777216;
	}

	touch_int.ToggleDirection();
	touch_int.Set(false);
	System::Systick::DelayMS(10);
	touch_rst.Set(true);
	System::Systick::DelayMS(10);

	touch_int.ToggleDirection();

	uint8_t touch_ic_version[10] = {};
	assert(i2c_master->GetBytes(GTP_ADDRESS, (uint16_t)GTP_REG_VERSION, (uint8_t *)touch_ic_version, 10));
	uint8_t config_length = sizeof(CTP_CFG_GT9157) / sizeof(CTP_CFG_GT9157[0]);
	uint8_t touch_config[GTP_CONFIG_MAX_LENGTH] = {};
	memset(touch_config, 0, GTP_CONFIG_MAX_LENGTH);
	memcpy(touch_config, CTP_CFG_GT9157, config_length);
	uint32_t check_sum = 0;
	for (uint32_t i = 0; i < 0x80FE - 0x8047 + 1; i++)
	{
		check_sum += touch_config[i];
	}
	touch_config[0x80FE - 0x8047 + 1] = (~check_sum) + 1;
	touch_config[0x80FE - 0x8047 + 1 + 1] = 1;
	for (uint8_t retry = 0; retry < 5; retry++)
	{
		if (i2c_master->SendBytes(GTP_ADDRESS, (uint16_t)GTP_REG_CONFIG_DATA, (uint8_t *)touch_config, 0x80FE - 0x8047 + 1 + 2))
		{
			break;
		}
	}
	System::Systick::DelayMS(500);

	uint8_t buf[188] = {};
	i2c_master->GetBytes(GTP_ADDRESS, (uint16_t)GTP_REG_CONFIG_DATA, (uint8_t *)buf, 188);

	disp.Set(true);

	elcdif.Start();

	bk.Set(true);
}

uint32_t *Firege5InchTouchScreen::LockFrameBuffer()
{
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	return (uint32_t *)(lcd_buffer[active_buffer]);
}

void Firege5InchTouchScreen::FillColor(const uint32_t color)
{
	if (region.x >= width || region.y >= height)
	{
		return;
	}
	uint32_t x, y, x_max, y_max;
	switch (orientation)
	{
	case 0:
		x = region.y;
		y = width - 1 - region.x + region.width;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 1:
		x = region.x;
		y = region.y;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	case 2:
		x = height - 1 - region.y - region.height;
		y = region.x;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 3:
		x = width - 1 - region.x - region.width;
		y = height - 1 - region.y - region.height;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	default:
		return;
	}
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	for (; y < y_max; y++)
	{
		for (; x < x_max; x++)
		{
			lcd_buffer[active_buffer][y * 800 + x] = color;
		}
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::FillColor(const Rect &rect, const uint32_t color)
{
	region = rect;
	if (region.x >= width || region.y >= height)
	{
		return;
	}
	uint32_t x, y, x_max, y_max;
	switch (orientation)
	{
	case 0:
		x = region.y;
		y = width - 1 - region.x + region.width;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 1:
		x = region.x;
		y = region.y;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	case 2:
		x = height - 1 - region.y - region.height;
		y = region.x;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 3:
		x = width - 1 - region.x - region.width;
		y = height - 1 - region.y - region.height;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	default:
		return;
	}
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	for (; y < y_max; y++)
	{
		for (uint32_t xtemp = x; xtemp < x_max; xtemp++)
		{
			lcd_buffer[active_buffer][y * 800 + xtemp] = color;
		}
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::FillGrayscalePixel(const uint8_t *pixel, const uint32_t length)
{
	if (region.x >= width || region.y >= height)
	{
		return;
	}
	uint32_t x, y, x_max, y_max;
	switch (orientation)
	{
	case 0:
		x = region.y;
		y = width - 1 - region.x + region.width;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 1:
		x = region.x;
		y = region.y;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	case 2:
		x = height - 1 - region.y - region.height;
		y = region.x;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 3:
		x = width - 1 - region.x - region.width;
		y = height - 1 - region.y - region.height;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	default:
		return;
	}
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	uint32_t count = 0;
	for (; y < y_max; y++)
	{
		for (uint32_t xtemp = x; xtemp < x_max; xtemp++)
		{
			uint8_t data = *(pixel + count);
			lcd_buffer[active_buffer][y * 800 + xtemp] = data << 16 | data << 8 | data;
			if (++count >= length)
			{
				return;
			}
		}
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::FillColorPixel(const uint16_t *pixel, const uint32_t length)
{
	if (region.x >= width || region.y >= height)
	{
		return;
	}
	uint32_t x, y, x_max, y_max;
	switch (orientation)
	{
	case 0:
		x = region.y;
		y = width - 1 - region.x + region.width;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 1:
		x = region.x;
		y = region.y;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	case 2:
		x = height - 1 - region.y - region.height;
		y = region.x;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 3:
		x = width - 1 - region.x - region.width;
		y = height - 1 - region.y - region.height;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	default:
		return;
	}
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	uint32_t count = 0;
	for (; y < y_max; y++)
	{
		for (uint32_t xtemp = x; xtemp < x_max; xtemp++)
		{
			uint16_t data = *(pixel + count);
			lcd_buffer[active_buffer][y * 800 + xtemp] = ((data << 8) & 0xF80000) | ((data << 5) & 0xFC00) | ((data << 3) & 0xF8);
			if (++count >= length)
			{
				return;
			}
		}
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::FillColorPixel(const uint32_t *pixel, const uint32_t length)
{
	if (region.x >= width || region.y >= height)
	{
		return;
	}
	uint32_t x, y, x_max, y_max;
	switch (orientation)
	{
	case 0:
		x = region.y;
		y = width - 1 - region.x + region.width;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 1:
		x = region.x;
		y = region.y;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	case 2:
		x = height - 1 - region.y - region.height;
		y = region.x;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 3:
		x = width - 1 - region.x - region.width;
		y = height - 1 - region.y - region.height;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	default:
		return;
	}
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	uint32_t count = 0;
	for (; y < y_max; y++)
	{
		for (uint32_t xtemp = x; xtemp < x_max; xtemp++)
		{
			lcd_buffer[active_buffer][y * 800 + xtemp] = *(pixel + count);
			if (++count >= length)
			{
				return;
			}
		}
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::FillBits(const uint32_t color_t, const uint32_t color_f, const uint8_t *data, const uint32_t bit_length)
{
	if (region.x >= width || region.y >= height)
	{
		return;
	}
	uint32_t x, y, x_max, y_max;
	switch (orientation)
	{
	case 0:
		x = region.y;
		y = width - 1 - region.x + region.width;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 1:
		x = region.x;
		y = region.y;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	case 2:
		x = height - 1 - region.y - region.height;
		y = region.x;
		x_max = x + region.height;
		y_max = y + region.width;
		break;
	case 3:
		x = width - 1 - region.x - region.width;
		y = height - 1 - region.y - region.height;
		x_max = x + region.width;
		y_max = y + region.height;
		break;
	default:
		return;
	}
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	uint32_t count = 0;
	for (; y < y_max; y++)
	{
		for (uint32_t xtemp = x; xtemp < x_max; xtemp++)
		{
			lcd_buffer[active_buffer][y * 800 + xtemp] = (*(data + count)) ? color_t : color_f;
			if (++count >= bit_length)
			{
				return;
			}
		}
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::Clear()
{
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	for (uint32_t i = 0; i < 384000; ++i)
	{
		lcd_buffer[active_buffer][i] = 0;
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::Clear(const uint32_t color)
{
	buffer_writing = true;
	if (buffer_swapped)
	{
		BufferCopy();
	}
	buffer_changed = true;
	for (uint32_t i = 0; i < 384000; ++i)
	{
		lcd_buffer[active_buffer][i] = color;
	}
	buffer_writing = false;
}

void Firege5InchTouchScreen::BufferCopy()
{
	uint8_t next = active_buffer ^ 1;
	for (uint32_t i = 0; i < 384000; ++i)
	{
		lcd_buffer[active_buffer][i] = lcd_buffer[next][i];
	}
	buffer_swapped = false;
}

uint32_t Firege5InchTouchScreen::GetTouchLocation(int32_t *x, int32_t *y)
{
	//	if (touch_int.Get()) {
	uint8_t point_data[42] = {};
	uint8_t touch_num = 0;
	uint8_t finger = 0;

	uint8_t *coor_data = nullptr;
	// int32_t input_x = 0;
	// int32_t input_y = 0;
	// int32_t input_w = 0;
	uint8_t id = 0;

	if (!i2c_master->GetBytes(GTP_ADDRESS, (uint16_t)GTP_READ_COOR_ADDR, (uint8_t *)point_data, 10))
	{
		return 0;
	}

	finger = point_data[0];

	if (finger == 0x00)
	{
		return 0;
	}

	if ((finger & 0x80) == 0) //�ж�buffer statusλ
	{
		goto exit_work_func;
		//����δ������������Ч
	}

	touch_num = finger & 0x0f; //�������
	if (touch_num > 5)
	{
		goto exit_work_func;
		//�������֧�ֵ����������˳�
	}

	if (touch_num > 1) //��ֹһ����
	{
		uint8_t *buf = (uint8_t *)0x1;
		if (!i2c_master->GetBytes(GTP_ADDRESS, (uint16_t)(GTP_READ_COOR_ADDR + 10), (uint8_t *)buf, 8 * (touch_num - 1)))
		{
			return 0;
		}
		memcpy(&point_data[10], buf, 8 * (touch_num - 1)); //����������������ݵ�point_data
	}

	if (touching_num > touch_num)
	{ //pre_touch>touch_num,��ʾ�еĵ��ͷ���
		for (uint8_t i = 0; i < touching_num; i++)
		{
			for (uint8_t j = 0; j < touch_num; j++)
			{
				id = point_data[j * 8 + 3] & 0x0F;
				if (touching_id[i] == id)
					break;
				if (j >= touch_num - 1)
				{
					touch_x_coor.erase(touch_x_coor.begin() + i);
					touch_y_coor.erase(touch_y_coor.begin() + i);
					touching_id.erase(touching_id.begin() + i);
					break;
				}
			}
		}
		//		for (uint8_t i = 0; i < touching_num; i++)						//һ����һ���㴦��
		//				{
		//			for (uint8_t j = 0; j < touch_num; j++) {
		//				coor_data = &point_data[j * 8 + 3];
		//				id = coor_data[0] & 0x0F;									//track id
		//				if (touching_id[i] == id)
		//					break;
		//
		//				if (j >= touch_num - 1)											//������ǰ����id���Ҳ���pre_id[i]����ʾ���ͷ�
		//						{
		//					touch_x_coor[touching_id[i]] = -1;
		//					touch_y_coor[touching_id[i]] = -1;
		//					touching_id.erase(touching_id.begin() + i);
		//				}
		//			}
		//		}
	}

	if (touch_num)
	{
		for (uint8_t i = 0; i < touch_num; i++)
		{
			coor_data = &point_data[i * 8 + 1];
			id = coor_data[0] & 0x0F;
			bool updated = false;
			for (uint8_t j = 0; j < touching_id.size(); j++)
			{
				if (touching_id[j] == id)
				{
					touch_x_coor[j] = coor_data[1] | (coor_data[2] << 8);
					touch_y_coor[j] = coor_data[3] | (coor_data[4] << 8);
					updated = true;
					break;
				}
			}
			if (!updated)
			{
				touching_id.push_back(id);
				touch_x_coor.push_back(coor_data[1] | (coor_data[2] << 8));
				touch_y_coor.push_back(coor_data[3] | (coor_data[4] << 8));
			}
		}
		//		for (uint8_t i = 0; i < touch_num; i++)						//һ����һ���㴦��
		//				{
		//			coor_data = &point_data[i * 8 + 1];
		//
		//			id = coor_data[0] & 0x0F;									//track id
		//			touching_id[i] = id;
		//
		//			input_x = coor_data[1] | (coor_data[2] << 8);	//x����
		//			input_y = coor_data[3] | (coor_data[4] << 8);	//y����
		//			input_w = coor_data[5] | (coor_data[6] << 8);	//size
		//
		//			{
		//				touch_x_coor[id] = input_x;
		//				touch_y_coor[id] = input_y;
		//			}
		//		}
	}
	//	else if (touching_num)		//touch_ num=0 ��pre_touch��=0
	//	{
	//		for (uint8_t i = 0; i < touching_num; i++) {
	//			touch_x_coor[touching_id[i]] = -1;
	//			touch_y_coor[touching_id[i]] = -1;
	//		}
	//	}

	touching_num = touch_num;

exit_work_func:
{
	if (!i2c_master->SendByte(GTP_ADDRESS, (uint16_t)GTP_READ_COOR_ADDR, 0))
	{
		return 0;
	}
}
	if (touching_num)
	{
		for (uint8_t i = 0; i < touching_num; ++i)
		{
			x[i] = touch_x_coor[i];
			y[i] = touch_y_coor[i];
		}
	}
	return touching_num;
	//	} else {
	//		return 0;
	//	}
}

uint32_t Firege5InchTouchScreen::GetActiveTouchNum()
{
	if (!touch_int.Get())
	{
		return touching_num;
	}
	else
	{
		return 0;
	}
}

Firege5InchTouchScreen::Gesture Firege5InchTouchScreen::GetGesture(uint32_t &param_1, uint32_t &param_2)
{
	uint8_t prev_touching = touching_num;
	std::vector<uint8_t> prev_id = touching_id;
	int32_t x[5];
	int32_t y[5];
	GetTouchLocation(x, y);
	if (touching_num > prev_touching)
	{
		for (int i = 0; i < touching_num - prev_touching; i++)
		{
			uint8_t id = touching_id[prev_touching + i];
			gesture_x_coor[id] = x[prev_touching + i];
			gesture_y_coor[id] = y[prev_touching + i];
			point_gesture[id] = Gesture::kDisable;
		}
	}
	int32_t gesture_x_param[5] = {};
	int32_t gesture_y_param[5] = {};
	int32_t dx, dy, dx2, dy2;
	int32_t zoom_original_x[2] = {};
	int32_t zoom_original_y[2] = {};
	int32_t zoom_now_x[2] = {};
	int32_t zoom_now_y[2] = {};
	uint8_t point_count = 0;
	for (int i = 0; i < touching_num; i++)
	{
		uint8_t id = touching_id[i];
		dx = touch_x_coor[i] - gesture_x_coor[id];
		dy = touch_y_coor[i] - gesture_y_coor[id];
		dx2 = dx * dx;
		dy2 = dy * dy;
		if (dx2 + dy2 > 400)
		{
			point_gesture[id] = Gesture::kDrag;
			gesture_x_param[id] = dx;
			gesture_y_param[id] = dy;
			zoom_original_x[point_count] = gesture_x_coor[id];
			zoom_original_y[point_count] = gesture_y_coor[id];
			zoom_now_x[point_count] = touch_x_coor[id];
			zoom_now_y[point_count] = touch_y_coor[id];
			gesture_x_coor[id] = touch_x_coor[i];
			gesture_y_coor[id] = touch_y_coor[i];
			if (++point_count > 1)
			{
				point_count = 0;
			}
		}
	}
	if (prev_touching > touching_num)
	{
		for (int i = 0; i < prev_touching - touching_num; i++)
		{
			uint8_t id = prev_id[touching_num + i];
			if (point_gesture[id] == Gesture::kDisable)
			{
				point_gesture[id] = Gesture::kTap;
			}
		}
	}
	else if (!touching_num)
	{
		for (int i = 0; i < 5; i++)
		{
			point_gesture[i] = Gesture::kDisable;
		}
	}
	Gesture return_gesture = Gesture::kDisable;
	std::vector<int32_t> return_param_1, return_param_2;
	for (int i = 0; i < 5; i++)
	{
		if (return_gesture == Gesture::kDisable && point_gesture[i] == Gesture::kTap)
		{
			point_gesture[i] = Gesture::kDisable;
			if (return_gesture == Gesture::kDisable)
			{
				return_gesture = Gesture::kTap;
				param_1 = gesture_x_coor[i];
				param_2 = gesture_y_coor[i];
			}
		}
		else if (return_gesture != Gesture::kDrag && point_gesture[i] == Gesture::kDrag)
		{
			return_gesture = Gesture::kDrag;
			point_gesture[i] = Gesture::kDisable;
			return_param_1.push_back(gesture_x_param[i]);
			return_param_2.push_back(gesture_y_param[i]);
		}
		else if (point_gesture[i] == Gesture::kDrag)
		{
			return_gesture = Gesture::kZoomIn;
			point_gesture[i] = Gesture::kDisable;
			return_param_1.push_back(gesture_x_param[i]);
			return_param_2.push_back(gesture_y_param[i]);
		}
	}
	if (return_gesture == Gesture::kDrag)
	{
		if (std::abs(dx) > std::abs(dy))
		{
			param_1 = std::abs(dx);
			return dx > 0 ? Gesture::kSwipeRight : Gesture::kSwipeLeft;
		}
		else
		{
			param_2 = std::abs(dy);
			return dy > 0 ? Gesture::kSwipeDown : Gesture::kSwipeUp;
		}
	}
	else if (return_gesture == Gesture::kZoomIn)
	{
		float mag_product = sqrt(return_param_1[0] * return_param_1[0] + return_param_2[0] * return_param_2[0]) * sqrt(return_param_1[1] * return_param_1[1] + return_param_2[1] * return_param_2[1]);
		float angle_diff = acos((float)(return_param_1[0] * return_param_1[1] + return_param_2[0] * return_param_2[1]) / mag_product);
		if (angle_diff > 1.91986218)
		{
			float original_distance = (zoom_original_x[0] - zoom_original_x[1]) * (zoom_original_x[0] - zoom_original_x[1]) + (zoom_original_y[0] - zoom_original_y[1]) * (zoom_original_y[0] - zoom_original_y[1]);
			float current_distance = (zoom_now_x[0] - zoom_now_x[1]) * (zoom_now_x[0] - zoom_now_x[1]) + (zoom_now_y[0] - zoom_now_y[1]) * (zoom_now_y[0] - zoom_now_y[1]);
			if (current_distance > original_distance)
			{
				param_1 = sqrt(current_distance) - sqrt(original_distance);
				return_gesture = Gesture::kZoomIn;
			}
			else
			{
				param_1 = sqrt(original_distance) - sqrt(current_distance);
				return_gesture = Gesture::kZoomOut;
			}
		}
		else
		{
			return_gesture = Gesture::kDisable;
		}
	}
	return return_gesture;
}

} // namespace DeviceDriver
