/*
 * MT9V034.cpp
 *
 *  Created on: Sep 15, 2018
 *      Author: LeeChunHei
 */

#include "device_driver/MT9V034.h"
#include "driver/soft_i2c_master.h"
#include "driver/hard_i2c_master.h"
#include <assert.h>

#ifdef MT9V034_USED

namespace DeviceDriver
{

__attribute__((section("NonCacheable,\"aw\",%nobits @"))) static uint8_t frame_buffer[4][360960] __attribute__((aligned(64)));

const System::Pinout::Name mt9v034_pins[MT9V034_USED * 15] = MT9V034_PINS;

const Driver::I2CMaster::Config MT9V034::GetI2CConfig(System::Pinout::Name sck, System::Pinout::Name sda)
{
	Driver::I2CMaster::Config config;
	config.scl = sck;
	config.sda = sda;
	config.send_wait_time = 200000;
	config.recieve_wait_time = 200000;
	config.debug_enable = true;
	config.baud_rate_Hz = 100000;
	return config;
}

const Driver::Csi::Config MT9V034::GetCsiConfig(const MT9V034::Config &camera_config)
{
	assert(MT9V034_DATABUS == 8 || MT9V034_DATABUS == 10 || MT9V034_DATABUS == 16);
	Driver::Csi::Config config;
#if MT9V034_DATABUS == 10
	config.pin_list.data0 = mt9v034_pins[camera_config.id * 15 + 5];
	config.pin_list.data1 = mt9v034_pins[camera_config.id * 15 + 6];
#endif
	config.pin_list.data2 = mt9v034_pins[camera_config.id * 15 + 7];
	config.pin_list.data3 = mt9v034_pins[camera_config.id * 15 + 8];
	config.pin_list.data4 = mt9v034_pins[camera_config.id * 15 + 9];
	config.pin_list.data5 = mt9v034_pins[camera_config.id * 15 + 10];
	config.pin_list.data6 = mt9v034_pins[camera_config.id * 15 + 11];
	config.pin_list.data7 = mt9v034_pins[camera_config.id * 15 + 12];
	config.pin_list.data8 = mt9v034_pins[camera_config.id * 15 + 13];
	config.pin_list.data9 = mt9v034_pins[camera_config.id * 15 + 14];
	config.pin_list.pclk = mt9v034_pins[camera_config.id * 15 + 4];
	config.pin_list.vsync = mt9v034_pins[camera_config.id * 15 + 2];
	config.pin_list.hsync = mt9v034_pins[camera_config.id * 15 + 3];
	config.is_10_bit_data = (MT9V034_DATABUS == 10);
	config.bytes_per_pixel = MT9V034_DATABUS == 8 ? 1 : 2;
	config.data_bus = Driver::Csi::Config::DataBus::k8Bit;
	volatile uint16_t width = camera_config.width >> 3;
	config.width = width << 3;
	config.height = camera_config.height;
	config.work_mode = Driver::Csi::Config::WorkMode::kGatedClockMode;
	config.line_pitch_bytes = config.width * config.bytes_per_pixel;
	config.vsync_active_low = false;
	return config;
}

void MT9V034::RegSet(uint8_t reg_addr, uint16_t value, bool check)
{
	do
	{
		assert(i2c_master->SendByte(0xB8 >> 1, (uint8_t)reg_addr, value >> 8));
		assert(i2c_master->SendByte(0xB8 >> 1, (uint8_t)0xF0, value & 0xFF));
	} while (check && RegGet(reg_addr)!=value);
}

uint16_t MT9V034::RegGet(uint8_t reg_addr){
	uint8_t out_byte_high, out_byte_low;
	uint16_t result;
	assert(i2c_master->GetByte(0xB8 >> 1, (uint8_t)reg_addr, out_byte_high));
	assert(i2c_master->GetByte(0xB8 >> 1, (uint8_t)0xF0, out_byte_low));
	result = out_byte_high;
	result <<= 8;
	result += out_byte_low;
	return result;
}

MT9V034::MT9V034(const Config &config) : csi(GetCsiConfig(config)), width((config.width >> 3) << 3), height(config.height)
{
	if (config.i2c_master)
	{
		i2c_master = config.i2c_master;
	}
	else
	{
		System::Pinout::Config scl_config, sda_config;
		uint8_t scl_module, sda_module;
		scl_config.pin = mt9v034_pins[config.id * 15 + 1];
		sda_config.pin = mt9v034_pins[config.id * 15];
		Driver::I2CMaster::Config i2c_config = GetI2CConfig(scl_config.pin, sda_config.pin);
		if (!System::Pinout::GetI2CSclPinConfig(scl_config, scl_module) || !System::Pinout::GetI2CSdaPinConfig(sda_config, sda_module) || (scl_module != sda_module))
		{
			i2c_master = new Driver::SoftI2CMaster(i2c_config);
		}
		else
		{
			i2c_master = new Driver::HardI2CMaster(i2c_config);
		}
	}

	RegSet(0xFE, 0xBEEF, false);

	assert(RegGet(0x00) == 0x1324);

	//Reset control circuit
	RegSet(0x0C, 1, false);
	RegSet(0x0C, 0, false);

	//Load 
	RegSet(0x01, 0x0001,false);   //COL_WINDOW_START_CONTEXTA_REG
    RegSet(0x02, 0x0004,false);   //ROW_WINDOW_START_CONTEXTA_REG
    RegSet(0x03, 0x01E0,false);   //ROW_WINDOW_SIZE_CONTEXTA_REG
    RegSet(0x04, 0x02F0,false);   //COL_WINDOW_SIZE_CONTEXTA_REG
    RegSet(0x05, 0x005E,false);   //HORZ_BLANK_CONTEXTA_REG
    RegSet(0x06, 0x002D,false);   //VERT_BLANK_CONTEXTA_REG
    RegSet(0x07, 0x0188,false);   //CONTROL_MODE_REG
    RegSet(0x08, 0x01BB,false);   //COARSE_SHUTTER_WIDTH_1_CONTEXTA
    RegSet(0x09, 0x01D9,false);   //COARSE_SHUTTER_WIDTH_2_CONTEXTA
    RegSet(0x0A, 0x0164,false);   //SHUTTER_WIDTH_CONTROL_CONTEXTA
    RegSet(0x0B, 0x0000,false);   //COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
    RegSet(0x0C, 0x0000,false);   //RESET_REG
    RegSet(0x0D, 0x0300,false);   //READ_MODE_REG
    RegSet(0x0E, 0x0000,false);   //READ_MODE2_REG
    RegSet(0x0F, 0x0000,false);   //PIXEL_OPERATION_MODE
    RegSet(0x10, 0x0040,false);   //RAMP_START_DELAY
    RegSet(0x11, 0x8042,false);   //OFFSET_CONTROL
    RegSet(0x12, 0x0022,false);   //AMP_RESET_BAR_CONTROL
    RegSet(0x13, 0x2D2E,false);   //5T_PIXEL_RESET_CONTROL
    RegSet(0x14, 0x0E02,false);   //4T_PIXEL_RESET_CONTROL
    RegSet(0x15, 0x0E32,false);   //TX_CONTROL
    RegSet(0x16, 0x2802,false);   //5T_PIXEL_SHS_CONTROL
    RegSet(0x17, 0x3E38,false);   //4T_PIXEL_SHS_CONTROL
    RegSet(0x18, 0x3E38,false);   //5T_PIXEL_SHR_CONTROL
    RegSet(0x19, 0x2802,false);   //4T_PIXEL_SHR_CONTROL
    RegSet(0x1A, 0x0428,false);   //COMPARATOR_RESET_CONTROL
    RegSet(0x1B, 0x0000,false);   //LED_OUT_CONTROL
    RegSet(0x1C, 0x0302,false);   //DATA_COMPRESSION
    RegSet(0x1D, 0x0040,false);   //ANALOG_TEST_CONTROL
    RegSet(0x1E, 0x0000,false);   //SRAM_TEST_DATA_ODD
    RegSet(0x1F, 0x0000,false);   //SRAM_TEST_DATA_EVEN
    RegSet(0x20, 0x03C7,false);   //BOOST_ROW_EN
    RegSet(0x21, 0x0020,false);   //I_VLN_CONTROL
    RegSet(0x22, 0x0020,false);   //I_VLN_AMP_CONTROL
    RegSet(0x23, 0x0010,false);   //I_VLN_CMP_CONTROL
    RegSet(0x24, 0x001B,false);   //I_OFFSET_CONTROL
    RegSet(0x26, 0x0004,false);   //I_VLN_VREF_ADC_CONTROL
    RegSet(0x27, 0x000C,false);   //I_VLN_STEP_CONTROL
    RegSet(0x28, 0x0010,false);   //I_VLN_BUF_CONTROL
    RegSet(0x29, 0x0010,false);   //I_MASTER_CONTROL
    RegSet(0x2A, 0x0020,false);   //I_VLN_AMP_60MHZ_CONTROL
    RegSet(0x2B, 0x0004,false);   //VREF_AMP_CONTROL
    RegSet(0x2C, 0x0004,false);   //VREF_ADC_CONTROL
    RegSet(0x2D, 0x0004,false);   //VBOOST_CONTROL
    RegSet(0x2E, 0x0007,false);   //V_HI_CONTROL
    RegSet(0x2F, 0x0003,false);   //V_LO_CONTROL
    RegSet(0x30, 0x0003,false);   //V_AMP_CAS_CONTROL
    RegSet(0x31, 0x0027,false);   //V1_CONTROL_CONTEXTA
    RegSet(0x32, 0x001A,false);   //V2_CONTROL_CONTEXTA
    RegSet(0x33, 0x0005,false);   //V3_CONTROL_CONTEXTA
    RegSet(0x34, 0x0003,false);   //V4_CONTROL_CONTEXTA
    RegSet(0x35, 0x0010,false);   //GLOBAL_GAIN_CONTEXTA_REG
    RegSet(0x36, 0x8010,false);   //GLOBAL_GAIN_CONTEXTB_REG
    RegSet(0x37, 0x0000,false);   //VOLTAGE_CONTROL
    RegSet(0x38, 0x0000,false);   //IDAC_VOLTAGE_MONITOR
    RegSet(0x39, 0x0027,false);   //V1_CONTROL_CONTEXTB
    RegSet(0x3A, 0x0026,false);   //V2_CONTROL_CONTEXTB
    RegSet(0x3B, 0x0005,false);   //V3_CONTROL_CONTEXTB
    RegSet(0x3C, 0x0003,false);   //V4_CONTROL_CONTEXTB
    RegSet(0x40, 0x0080,false);   //DARK_AVG_THRESHOLDS
    RegSet(0x46, 0x231D,false);   //CALIB_CONTROL_REG (AUTO)
    RegSet(0x47, 0x0080,false);   //STEP_SIZE_AVG_MODE
    RegSet(0x48, 0x0020,false);   //ROW_NOISE_CONTROL
    RegSet(0x4C, 0x0002,false);   //NOISE_CONSTANT
    RegSet(0x60, 0x0000,false);   //PIXCLK_CONTROL
    RegSet(0x67, 0x0000,false);   //TEST_DATA
    RegSet(0x6C, 0x0000,false);   //TILE_X0_Y0
    RegSet(0x70, 0x0000,false);   //TILE_X1_Y0
    RegSet(0x71, 0x002A,false);   //TILE_X2_Y0
    RegSet(0x72, 0x0000,false);   //TILE_X3_Y0
    RegSet(0x7F, 0x0000,false);   //TILE_X4_Y0
    RegSet(0x99, 0x0000,false);   //TILE_X0_Y1
    RegSet(0x9A, 0x0096,false);   //TILE_X1_Y1
    RegSet(0x9B, 0x012C,false);   //TILE_X2_Y1
    RegSet(0x9C, 0x01C2,false);   //TILE_X3_Y1
    RegSet(0x9D, 0x0258,false);   //TILE_X4_Y1
    RegSet(0x9E, 0x02F0,false);   //TILE_X0_Y2
    RegSet(0x9F, 0x0000,false);   //TILE_X1_Y2
    RegSet(0xA0, 0x0060,false);   //TILE_X2_Y2
    RegSet(0xA1, 0x00C0,false);   //TILE_X3_Y2
    RegSet(0xA2, 0x0120,false);   //TILE_X4_Y2
    RegSet(0xA3, 0x0180,false);   //TILE_X0_Y3
    RegSet(0xA4, 0x01E0,false);   //TILE_X1_Y3
    RegSet(0xA5, 0x003A,false);   //TILE_X2_Y3
    RegSet(0xA6, 0x0002,false);   //TILE_X3_Y3
    RegSet(0xA8, 0x0000,false);   //TILE_X4_Y3
    RegSet(0xA9, 0x0002,false);   //TILE_X0_Y4
    RegSet(0xAA, 0x0002,false);   //TILE_X1_Y4
    RegSet(0xAB, 0x0040,false);   //TILE_X2_Y4
    RegSet(0xAC, 0x0001,false);   //TILE_X3_Y4
    RegSet(0xAD, 0x01E0,false);   //TILE_X4_Y4
    RegSet(0xAE, 0x0014,false);   //X0_SLASH5
    RegSet(0xAF, 0x0000,false);   //X1_SLASH5
    RegSet(0xB0, 0xABE0,false);   //X2_SLASH5
    RegSet(0xB1, 0x0002,false);   //X3_SLASH5
    RegSet(0xB2, 0x0010,false);   //X4_SLASH5
    RegSet(0xB3, 0x0010,false);   //X5_SLASH5
    RegSet(0xB4, 0x0000,false);   //Y0_SLASH5
    RegSet(0xB5, 0x0000,false);   //Y1_SLASH5
    RegSet(0xB6, 0x0000,false);   //Y2_SLASH5
    RegSet(0xB7, 0x0000,false);   //Y3_SLASH5
    RegSet(0xBF, 0x0016,false);   //Y4_SLASH5
    RegSet(0xC0, 0x000A,false);   //Y5_SLASH5
    RegSet(0xC2, 0x18D0,false);   //DESIRED_BIN
    RegSet(0xC3, 0x007F,false);   //EXP_SKIP_FRM_H
    RegSet(0xC4, 0x007F,false);   //EXP_LPF
    RegSet(0xC5, 0x007F,false);   //GAIN_SKIP_FRM
    RegSet(0xC6, 0x0000,false);   //GAIN_LPF_H
    RegSet(0xC7, 0x4416,false);   //MAX_GAIN
    RegSet(0xC8, 0x4421,false);   //MIN_COARSE_EXPOSURE
    RegSet(0xC9, 0x0001,false);   //MAX_COARSE_EXPOSURE
    RegSet(0xCA, 0x0004,false);   //BIN_DIFF_THRESHOLD
    RegSet(0xCB, 0x01E0,false);   //AUTO_BLOCK_CONTROL
    RegSet(0xCC, 0x02F0,false);   //PIXEL_COUNT
    RegSet(0xCD, 0x005E,false);   //LVDS_MASTER_CONTROL
    RegSet(0xCE, 0x002D,false);   //LVDS_SHFT_CLK_CONTROL
    RegSet(0xCF, 0x01DE,false);   //LVDS_DATA_CONTROL
    RegSet(0xD0, 0x01DF,false);   //LVDS_DATA_STREAM_LATENCY
    RegSet(0xD1, 0x0164,false);   //LVDS_INTERNAL_SYNC
    RegSet(0xD2, 0x0001,false);   //LVDS_USE_10BIT_PIXELS
    RegSet(0xD3, 0x0000,false);   //STEREO_ERROR_CONTROL
    RegSet(0xD4, 0x0000,false);   //INTERLACE_FIELD_VBLANK
    RegSet(0xD5, 0x0104,false);   //IMAGE_CAPTURE_NUM
    RegSet(0xD6, 0x0000,false);   //ANALOG_CONTROLS
    RegSet(0xD7, 0x0000,false);   //AB_PULSE_WIDTH_REG
    RegSet(0xD8, 0x0000,false);   //TX_PULLUP_PULSE_WIDTH_REG
    RegSet(0xD9, 0x0000,false);   //RST_PULLUP_PULSE_WIDTH_REG
    RegSet(0xF0, 0x0000,false);   //NTSC_FV_CONTROL
    RegSet(0xFE, 0xBEEF,false);   //NTSC_HBLANK

	uint16_t data = 0;
	uint16_t width = config.width;
	uint16_t height = config.height;
	uint16_t exposure;
    if((height*4)<=480)
    {
        height *= 4;
        data |= 2;
		exposure = (config.fps > 193)? 193:((config.fps<1)?1:config.fps);        
        if(exposure > 132)
        {
            exposure = (uint16_t)(-2.0 * exposure + 504);
        }
        else
        {
            exposure = (uint16_t)(132.0 / exposure * 240);
        }
    }
    else if((height*2)<=480)
    {
        height *= 2;
        data |= 1;
		exposure = (config.fps > 112)? 112:((config.fps<1)?1:config.fps);        
        if(exposure > 66)
        {
            exposure = (uint16_t)(-5.2 * exposure + 822);
        }
        else
        {
            exposure = (uint16_t)(66.0 / exposure * 480);
        }
    }
    else 
    {
		exposure = (config.fps > 60)? 60: ((config.fps<1)?1:config.fps);
        exposure = (uint16_t)(60.0 / exposure * 480);
        
    }
    if((width*4)<=752 )
    {
        width *= 4;
        data |= 2<<2;
    }
    else if((width*2)<=752 )
    {
        width *= 2;
        data |= 1<<2;
    }
	//Row and column flip
    data |= 3<<4;
	RegSet(0x0D, data);
    RegSet(0x04, width);
	RegSet(0x03, height);
	RegSet(0x01, (752-width)/2+1);
	RegSet(0x02, (480-height)/2+4);
	RegSet(0x0B, exposure);

	//Enable anti eclipse
	RegSet(0xC2, 0x18D0);
	RegSet(0xAB, 0x0040);
	RegSet(0xB0, width*height);
	RegSet(0x1C, 0x0303);

	//Reg fix
	RegSet(0x13, 0x2D2E);
	RegSet(0x20, 0x03C7);
	RegSet(0x24, 0x0010);
	RegSet(0x2B, 0x0003);
	RegSet(0x2F, 0x0003);

	//Coarse shutter image width control
	RegSet(0x0A, 0x0164);
	RegSet(0x32, 0x001A);
	RegSet(0x0F, 0x0103);
	RegSet(0xA5, 60);
	RegSet(0x35, 0x8010);

	//AEC AGC
	uint8_t af_value = ((config.enable_AGC & 1) << 1) | (config.enable_AEC & 1) | 0;
	RegSet(0xAF, (af_value << 8) | af_value | 0);

	switch (config.hdr_mode)
	{
	case Config::HDR::kDisable:
		RegSet(0x08, 0x01BB);
		RegSet(0x09, 0x01D9);
		RegSet(0x0A, 0x0164);
		if(RegGet(0x0B)>0x01E0)
			RegSet(0x0B, 0x01E0);
		RegSet(0x0F, 0x0100);
		RegSet(0x35, 0x0010);
		break;

	case Config::HDR::k80dB:
		RegSet(0x0A, 0x0164);
		if(RegGet(0x0B)>0x03E8)
			RegSet(0x0B, 0x03E8);
		RegSet(0x0F, 0x0103);
		RegSet(0x35, 0x8010);
		break;

	case Config::HDR::k100dB:
		RegSet(0x0A, 0x0164);
		if(RegGet(0x0B)>0x03E8)
			RegSet(0x0B, 0x03E8);
		RegSet(0x0F, 0x0103);
		RegSet(0x35, 0x8010);
		break;

	default:
		break;
	}

	RegSet(0x70, 0x0303); //ROW_NOISE_CONTROL

	//Reset
	RegSet(0x0C, 0x03, false);

	//Lock all register
	RegSet(0xFE, 0xDEAD, false);

	csi.ConfigTransferBuffer(0, (uint32_t)frame_buffer[0]);
	csi.ConfigTransferBuffer(1, (uint32_t)frame_buffer[1]);

	waiting_buffer_addr = (uint32_t)frame_buffer[2];
	ready_buffer_addr = (uint32_t)frame_buffer[3];

	transferred =false;
	csi.SetListener([&](Driver::Csi *csi_ptr) {
		uint8_t state = csi_ptr->IsTransferComplete();
		if(state&1){
			volatile uint32_t temp = waiting_buffer_addr;
			waiting_buffer_addr = csi_ptr->GetTransferBuffer(0);
			csi_ptr->ConfigTransferBuffer(0, temp);
		}
		if(state&2){
			volatile uint32_t temp = waiting_buffer_addr;
			waiting_buffer_addr = csi_ptr->GetTransferBuffer(1);
			csi_ptr->ConfigTransferBuffer(1, temp);
		}
		transferred=true;
	},5);
}

void MT9V034::Start()
{
	is_started = true;
	csi.Start();
}

void MT9V034::Stop()
{
	is_started = false;
	csi.Stop();
}

} // namespace DeviceDriver

#endif
