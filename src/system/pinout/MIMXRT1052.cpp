/*
 * MIMXRT1052.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: LeeChunHei
 */

#include "system/pinout/MIMXRT1052.h"
#include "system/clock.h"
#include <assert.h>
#include <bitset>

namespace System
{
namespace Pinout
{

std::bitset<124> pin_active; //record which pin is using

struct eFlexPWMPinConfig
{
	Name pin_name;
	Config::MuxMode mux_mode;
	uint8_t mod_sub_channel;
};

const eFlexPWMPinConfig eflexpwm_pin_config[67] = {{Name::kGPIO_EMC_00, Config::MuxMode::kAlt1, (3 << 5 | 0 << 2 | 0)}, {Name::kGPIO_EMC_01, Config::MuxMode::kAlt1, (3 << 5 | 0 << 2 | 1)}, {Name::kGPIO_EMC_02, Config::MuxMode::kAlt1, (3 << 5 | 1 << 2 | 0)}, {Name::kGPIO_EMC_03, Config::MuxMode::kAlt1, (3 << 5 | 1 << 2 | 1)}, {Name::kGPIO_EMC_04, Config::MuxMode::kAlt1, (3 << 5 | 2 << 2 | 0)}, {Name::kGPIO_EMC_05, Config::MuxMode::kAlt1, (3 << 5 | 2 << 2 | 1)}, {Name::kGPIO_EMC_06, Config::MuxMode::kAlt1, (1 << 5 | 0 << 2 | 0)}, {Name::kGPIO_EMC_07, Config::MuxMode::kAlt1, (1 << 5 | 0 << 2 | 1)}, {Name::kGPIO_EMC_08, Config::MuxMode::kAlt1, (1 << 5 | 1 << 2 | 0)}, {Name::kGPIO_EMC_09, Config::MuxMode::kAlt1, (1 << 5 | 1 << 2 | 1)}, {Name::kGPIO_EMC_10, Config::MuxMode::kAlt1, (1 << 5 | 2 << 2 | 0)}, {Name::kGPIO_EMC_11, Config::MuxMode::kAlt1, (1 << 5 | 2 << 2 | 1)}, {Name::kGPIO_EMC_12, Config::MuxMode::kAlt4, (0 << 5 | 3 << 2 | 0)}, {Name::kGPIO_EMC_13, Config::MuxMode::kAlt4, (0 << 5 | 3 << 2 | 1)}, {Name::kGPIO_EMC_17, Config::MuxMode::kAlt1, (3 << 5 | 3 << 2 | 0)}, {Name::kGPIO_EMC_18, Config::MuxMode::kAlt1, (3 << 5 | 3 << 2 | 1)}, {Name::kGPIO_EMC_19, Config::MuxMode::kAlt1, (1 << 5 | 3 << 2 | 0)}, {Name::kGPIO_EMC_20, Config::MuxMode::kAlt1, (1 << 5 | 3 << 2 | 1)}, {Name::kGPIO_EMC_21, Config::MuxMode::kAlt1, (2 << 5 | 3 << 2 | 0)}, {Name::kGPIO_EMC_22, Config::MuxMode::kAlt1, (2 << 5 | 3 << 2 | 1)}, {Name::kGPIO_EMC_23, Config::MuxMode::kAlt1, (0 << 5 | 0 << 2 | 0)}, {Name::kGPIO_EMC_24, Config::MuxMode::kAlt1, (0 << 5 | 0 << 2 | 1)}, {Name::kGPIO_EMC_25, Config::MuxMode::kAlt1, (0 << 5 | 1 << 2 | 0)}, {Name::kGPIO_EMC_26, Config::MuxMode::kAlt1, (0 << 5 | 1 << 2 | 1)}, {Name::kGPIO_EMC_27, Config::MuxMode::kAlt1, (0 << 5 | 2 << 2 | 0)}, {Name::kGPIO_EMC_28, Config::MuxMode::kAlt1, (0 << 5 | 2 << 2 | 1)}, {Name::kGPIO_EMC_29, Config::MuxMode::kAlt1, (2 << 5 | 0 << 2 | 0)}, {Name::kGPIO_EMC_30, Config::MuxMode::kAlt1, (2 << 5 | 0 << 2 | 1)}, {Name::kGPIO_EMC_31, Config::MuxMode::kAlt1, (2 << 5 | 1 << 2 | 0)}, {Name::kGPIO_EMC_32, Config::MuxMode::kAlt1, (2 << 5 | 1 << 2 | 1)}, {Name::kGPIO_EMC_33, Config::MuxMode::kAlt1, (2 << 5 | 2 << 2 | 0)}, {Name::kGPIO_EMC_34, Config::MuxMode::kAlt1, (2 << 5 | 2 << 2 | 1)}, {Name::kGPIO_EMC_38, Config::MuxMode::kAlt1, (0 << 5 | 3 << 2 | 0)}, {Name::kGPIO_EMC_39, Config::MuxMode::kAlt1, (0 << 5 | 3 << 2 | 1)}, {Name::kGPIO_AD_B0_00, Config::MuxMode::kAlt0, (1 << 5 | 3 << 2 | 0)}, {Name::kGPIO_AD_B0_01, Config::MuxMode::kAlt0, (1 << 5 | 3 << 2 | 1)}, {Name::kGPIO_AD_B0_02, Config::MuxMode::kAlt4, (0 << 5 | 0 << 2 | 2)}, {Name::kGPIO_AD_B0_03, Config::MuxMode::kAlt4, (0 << 5 | 1 << 2 | 2)}, {Name::kGPIO_AD_B0_09, Config::MuxMode::kAlt1, (1 << 5 | 3 << 2 | 0)}, {Name::kGPIO_AD_B0_10, Config::MuxMode::kAlt1, (0 << 5 | 3 << 2 | 0)}, {Name::kGPIO_AD_B0_11, Config::MuxMode::kAlt1, (0 << 5 | 3 << 2 | 1)}, {Name::kGPIO_AD_B0_12, Config::MuxMode::kAlt4, (0 << 5 | 2 << 2 | 2)}, {Name::kGPIO_AD_B0_13, Config::MuxMode::kAlt4, (0 << 5 | 3 << 2 | 2)}, {Name::kGPIO_AD_B1_08, Config::MuxMode::kAlt1, (3 << 5 | 0 << 2 | 0)}, {Name::kGPIO_AD_B1_09, Config::MuxMode::kAlt1, (3 << 5 | 1 << 2 | 0)}, {Name::kGPIO_B0_06, Config::MuxMode::kAlt2, (1 << 5 | 0 << 2 | 0)}, {Name::kGPIO_B0_07, Config::MuxMode::kAlt2, (1 << 5 | 0 << 2 | 1)}, {Name::kGPIO_B0_08, Config::MuxMode::kAlt2, (1 << 5 | 1 << 2 | 0)}, {Name::kGPIO_B0_09, Config::MuxMode::kAlt2, (1 << 5 | 1 << 2 | 1)}, {Name::kGPIO_B0_10, Config::MuxMode::kAlt2, (1 << 5 | 2 << 2 | 0)}, {Name::kGPIO_B0_11, Config::MuxMode::kAlt2, (1 << 5 | 2 << 2 | 1)}, {Name::kGPIO_B1_00, Config::MuxMode::kAlt6, (0 << 5 | 3 << 2 | 0)}, {Name::kGPIO_B1_01, Config::MuxMode::kAlt6, (0 << 5 | 3 << 2 | 1)}, {Name::kGPIO_B1_02, Config::MuxMode::kAlt6, (1 << 5 | 3 << 2 | 0)}, {Name::kGPIO_B1_03, Config::MuxMode::kAlt6, (1 << 5 | 3 << 2 | 1)}, {Name::kGPIO_B1_14, Config::MuxMode::kAlt1, (3 << 5 | 2 << 2 | 0)}, {Name::kGPIO_B1_15, Config::MuxMode::kAlt1, (3 << 5 | 3 << 2 | 0)}, {Name::kGPIO_SD_B0_00, Config::MuxMode::kAlt1, (0 << 5 | 0 << 2 | 0)}, {Name::kGPIO_SD_B0_01, Config::MuxMode::kAlt1, (0 << 5 | 0 << 2 | 1)}, {Name::kGPIO_SD_B0_02, Config::MuxMode::kAlt1, (0 << 5 | 1 << 2 | 0)}, {Name::kGPIO_SD_B0_03, Config::MuxMode::kAlt1, (0 << 5 | 1 << 2 | 1)}, {Name::kGPIO_SD_B0_04, Config::MuxMode::kAlt1, (0 << 5 | 2 << 2 | 0)}, {Name::kGPIO_SD_B0_05, Config::MuxMode::kAlt1, (0 << 5 | 2 << 2 | 1)}, {Name::kGPIO_SD_B1_00, Config::MuxMode::kAlt2, (0 << 5 | 3 << 2 | 0)}, {Name::kGPIO_SD_B1_01, Config::MuxMode::kAlt2, (0 << 5 | 3 << 2 | 1)}, {Name::kGPIO_SD_B1_02, Config::MuxMode::kAlt2, (1 << 5 | 3 << 2 | 0)}, {Name::kGPIO_SD_B1_03, Config::MuxMode::kAlt2, (1 << 5 | 3 << 2 | 1)}};

struct ModulePinConfig
{
	Name pin_name;
	Config::MuxMode mux_mode;
	uint8_t module;
	uint32_t select_input_reg;
	uint8_t select_input_value;
};

const ModulePinConfig uart_tx_pin_config[18] = {{Name::kGPIO_EMC_13, Config::MuxMode::kAlt2, 2, 0x401F853CU, 0x1U}, {Name::kGPIO_EMC_19, Config::MuxMode::kAlt2, 3, 0x401F8544U, 0x1U}, {Name::kGPIO_EMC_23, Config::MuxMode::kAlt2, 4, 0x401F854CU, 0x0U}, {Name::kGPIO_EMC_25, Config::MuxMode::kAlt2, 5, 0x401F8554U, 0x0U}, {Name::kGPIO_EMC_31, Config::MuxMode::kAlt2, 6, 0x401F855CU, 0x1U}, {Name::kGPIO_EMC_38, Config::MuxMode::kAlt2, 7, 0x401F8564U, 0x2U}, {Name::kGPIO_AD_B0_02, Config::MuxMode::kAlt2, 5, 0x401F8554U, 0x1U}, {Name::kGPIO_AD_B0_12, Config::MuxMode::kAlt2, 0, 0, 0}, {Name::kGPIO_AD_B1_02, Config::MuxMode::kAlt2, 1, 0x401F8530U, 0x1U}, {Name::kGPIO_AD_B1_06, Config::MuxMode::kAlt2, 2, 0x401F853CU, 0x0U}, {Name::kGPIO_AD_B1_10, Config::MuxMode::kAlt2, 7, 0x401F8564U, 0x1U}, {Name::kGPIO_B0_08, Config::MuxMode::kAlt3, 2, 0x401F853CU, 0x2U}, {Name::kGPIO_B1_00, Config::MuxMode::kAlt2, 3, 0x401F8544U, 0x2U}, {Name::kGPIO_B1_12, Config::MuxMode::kAlt1, 4, 0x401F854CU, 0x1U}, {Name::kGPIO_SD_B0_04, Config::MuxMode::kAlt2, 7, 0x401F8564U, 0x0U}, {Name::kGPIO_SD_B1_00, Config::MuxMode::kAlt4, 3, 0x401F8544U, 0x0U}, {Name::kGPIO_SD_B1_08, Config::MuxMode::kAlt2, 6, 0x401F855CU, 0x0U}, {Name::kGPIO_SD_B1_11, Config::MuxMode::kAlt2, 1, 0x401F8530U, 0x0U}};

const ModulePinConfig uart_rx_pin_config[18] = {{Name::kGPIO_EMC_14, Config::MuxMode::kAlt2, 2, 0x401F8538U, 0x1U}, {Name::kGPIO_EMC_20, Config::MuxMode::kAlt2, 3, 0x401F8540U, 0x1U}, {Name::kGPIO_EMC_24, Config::MuxMode::kAlt2, 4, 0x401F8548U, 0x0U}, {Name::kGPIO_EMC_26, Config::MuxMode::kAlt2, 5, 0x401F8550U, 0x0U}, {Name::kGPIO_EMC_32, Config::MuxMode::kAlt2, 6, 0x401F8558U, 0x1U}, {Name::kGPIO_EMC_39, Config::MuxMode::kAlt2, 7, 0x401F8560U, 0x2U}, {Name::kGPIO_AD_B0_03, Config::MuxMode::kAlt2, 5, 0x401F8550U, 0x1U}, {Name::kGPIO_AD_B0_13, Config::MuxMode::kAlt2, 0, 0, 0}, {Name::kGPIO_AD_B1_03, Config::MuxMode::kAlt2, 1, 0x401F852CU, 0x1U}, {Name::kGPIO_AD_B1_07, Config::MuxMode::kAlt2, 2, 0x401F8538U, 0x0U}, {Name::kGPIO_AD_B1_11, Config::MuxMode::kAlt2, 7, 0x401F8560U, 0x1U}, {Name::kGPIO_B0_09, Config::MuxMode::kAlt3, 2, 0x401F8538U, 0x2U}, {Name::kGPIO_B1_01, Config::MuxMode::kAlt2, 3, 0x401F8540U, 0x2U}, {Name::kGPIO_B1_13, Config::MuxMode::kAlt1, 4, 0x401F8548U, 0x1U}, {Name::kGPIO_SD_B0_05, Config::MuxMode::kAlt2, 7, 0x401F8560U, 0x0U}, {Name::kGPIO_SD_B1_01, Config::MuxMode::kAlt4, 3, 0x401F8540U, 0x0U}, {Name::kGPIO_SD_B1_09, Config::MuxMode::kAlt2, 6, 0x401F8558U, 0x0U}, {Name::kGPIO_SD_B1_10, Config::MuxMode::kAlt2, 1, 0x401F852CU, 0x0U}};

const ModulePinConfig i2c_scl_pin_config[10] = {{Name::kGPIO_EMC_12, Config::MuxMode::kAlt2, 3, 0x401F84E4U, 0x0U}, {Name::kGPIO_EMC_22, Config::MuxMode::kAlt2, 2, 0x401F84DCU, 0x0U}, {Name::kGPIO_AD_B0_00, Config::MuxMode::kAlt4, 0, 0, 0}, {Name::kGPIO_AD_B0_12, Config::MuxMode::kAlt0, 3, 0x401F84E4U, 0x1U}, {Name::kGPIO_AD_B1_00, Config::MuxMode::kAlt3, 0, 0x401F84CCU, 0x1U}, {Name::kGPIO_AD_B1_07, Config::MuxMode::kAlt1, 2, 0x401F84DCU, 0x2U}, {Name::kGPIO_B0_04, Config::MuxMode::kAlt2, 1, 0x401F84D4U, 0x1U}, {Name::kGPIO_SD_B0_00, Config::MuxMode::kAlt2, 2, 0x401F84DCU, 0x1U}, {Name::kGPIO_SD_B1_04, Config::MuxMode::kAlt2, 0, 0x401F84CCU, 0x0U}, {Name::kGPIO_SD_B1_11, Config::MuxMode::kAlt3, 1, 0x401F84D4U, 0x0U}};

const ModulePinConfig i2c_sda_pin_config[10] = {{Name::kGPIO_EMC_11, Config::MuxMode::kAlt2, 3, 0x401F84E8U, 0x0U}, {Name::kGPIO_EMC_21, Config::MuxMode::kAlt2, 2, 0x401F84E0U, 0x0U}, {Name::kGPIO_AD_B0_01, Config::MuxMode::kAlt4, 0, 0, 0}, {Name::kGPIO_AD_B0_13, Config::MuxMode::kAlt0, 3, 0x401F84E8U, 0x1U}, {Name::kGPIO_AD_B1_01, Config::MuxMode::kAlt3, 0, 0x401F84D0U, 0x1U}, {Name::kGPIO_AD_B1_06, Config::MuxMode::kAlt1, 2, 0x401F84E0U, 0x2U}, {Name::kGPIO_B0_05, Config::MuxMode::kAlt2, 1, 0x401F84D8U, 0x1U}, {Name::kGPIO_SD_B0_01, Config::MuxMode::kAlt2, 2, 0x401F84E0U, 0x1U}, {Name::kGPIO_SD_B1_05, Config::MuxMode::kAlt2, 0, 0x401F84D0U, 0x0U}, {Name::kGPIO_SD_B1_10, Config::MuxMode::kAlt3, 1, 0x401F84D8U, 0x0U}};

struct XBARPinConfig
{
	Name pin_name;
	Config::MuxMode mux_mode;
	uint32_t select_input_reg;
	uint8_t select_input_value;
};

const XBARPinConfig xbar_pin_config[46] = {{Name::kGPIO_EMC_00, Config::MuxMode::kAlt3, 0x401F860CU, 0x0U}, {Name::kGPIO_EMC_01, Config::MuxMode::kAlt3, 0x401F8610U, 0x0U}, {Name::kGPIO_EMC_02, Config::MuxMode::kAlt3, 0x401F8614U, 0x0U}, {Name::kGPIO_EMC_03, Config::MuxMode::kAlt3, 0x401F8618U, 0x0U}, {Name::kGPIO_EMC_04, Config::MuxMode::kAlt3, 0x401F861CU, 0x0U}, {Name::kGPIO_EMC_05, Config::MuxMode::kAlt3, 0x401F8620U, 0x0U}, {Name::kGPIO_EMC_06, Config::MuxMode::kAlt3, 0x401F8624U, 0x0U}, {Name::kGPIO_EMC_07, Config::MuxMode::kAlt3, 0x401F8628U, 0x0U}, {Name::kGPIO_EMC_08, Config::MuxMode::kAlt3, 0x401F862CU, 0x0U}, {Name::kGPIO_EMC_12, Config::MuxMode::kAlt1, 0x401F8640U, 0x0U}, {Name::kGPIO_EMC_13, Config::MuxMode::kAlt1, 0x401F8650U, 0x1U}, {Name::kGPIO_EMC_14, Config::MuxMode::kAlt1, 0x401F8654U, 0x0U}, {Name::kGPIO_EMC_15, Config::MuxMode::kAlt1, 0x401F8634U, 0x0U}, {Name::kGPIO_EMC_16, Config::MuxMode::kAlt1, 0x401F8658U, 0x0U}, {Name::kGPIO_EMC_35, Config::MuxMode::kAlt1, 0x401F8630U, 0x0U}, {Name::kGPIO_EMC_36, Config::MuxMode::kAlt1, 0x401F8638U, 0x0U}, {Name::kGPIO_EMC_37, Config::MuxMode::kAlt1, 0x401F863CU, 0x0U}, {Name::kGPIO_AD_B0_00, Config::MuxMode::kAlt1, 0x401F8644U, 0x0U}, {Name::kGPIO_AD_B0_01, Config::MuxMode::kAlt1, 0x401F8648U, 0x0U}, {Name::kGPIO_AD_B0_02, Config::MuxMode::kAlt1, 0x401F864CU, 0x0U}, {Name::kGPIO_AD_B0_03, Config::MuxMode::kAlt1, 0x401F862CU, 0x1U}, {Name::kGPIO_AD_B0_05, Config::MuxMode::kAlt6, 0x401F862CU, 0x2U}, {Name::kGPIO_AD_B0_06, Config::MuxMode::kAlt6, 0x401F8630U, 0x1U}, {Name::kGPIO_AD_B0_07, Config::MuxMode::kAlt6, 0x401F8654U, 0x1U}, {Name::kGPIO_AD_B0_08, Config::MuxMode::kAlt6, 0x401F8634U, 0x1U}, {Name::kGPIO_AD_B0_09, Config::MuxMode::kAlt6, 0x401F8658U, 0x1U}, {Name::kGPIO_AD_B0_10, Config::MuxMode::kAlt6, 0x401F8638U, 0x1U}, {Name::kGPIO_AD_B0_11, Config::MuxMode::kAlt6, 0x401F863CU, 0x1U}, {Name::kGPIO_AD_B0_14, Config::MuxMode::kAlt1, 0x401F8640U, 0x1U}, {Name::kGPIO_AD_B0_15, Config::MuxMode::kAlt1, 0x401F8650U, 0x0U}, {Name::kGPIO_B0_12, Config::MuxMode::kAlt1, 0, 0}, {Name::kGPIO_B0_13, Config::MuxMode::kAlt1, 0, 0}, {Name::kGPIO_B0_14, Config::MuxMode::kAlt1, 0, 0}, {Name::kGPIO_B0_15, Config::MuxMode::kAlt1, 0, 0}, {Name::kGPIO_B1_00, Config::MuxMode::kAlt1, 0x401F8644U, 0x1U}, {Name::kGPIO_B1_01, Config::MuxMode::kAlt1, 0x401F8648U, 0x1U}, {Name::kGPIO_B1_02, Config::MuxMode::kAlt1, 0x401F864CU, 0x1U}, {Name::kGPIO_B1_03, Config::MuxMode::kAlt1, 0x401F862CU, 0x3U}, {Name::kGPIO_B1_14, Config::MuxMode::kAlt3, 0x401F860CU, 0x1U}, {Name::kGPIO_B1_15, Config::MuxMode::kAlt3, 0x401F8610U, 0x1U}, {Name::kGPIO_SD_B0_00, Config::MuxMode::kAlt3, 0x401F8614U, 0x1U}, {Name::kGPIO_SD_B0_01, Config::MuxMode::kAlt3, 0x401F8618U, 0x1U}, {Name::kGPIO_SD_B0_02, Config::MuxMode::kAlt3, 0x401F861CU, 0x1U}, {Name::kGPIO_SD_B0_03, Config::MuxMode::kAlt3, 0x401F8620U, 0x1U}, {Name::kGPIO_SD_B0_04, Config::MuxMode::kAlt3, 0x401F8624U, 0x1U}, {Name::kGPIO_SD_B0_05, Config::MuxMode::kAlt3, 0x401F8628U, 0x1U}};

bool GetADCPinConfig(Config &config)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	config.mux_mode = Config::MuxMode::kAlt5;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kDisable;
	config.force_input = false;
	return true;
}

bool GeteFlexPWMPinConfig(Config &config, uint8_t &module, uint8_t &sub_module, uint8_t &channel)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin_name = config.pin;
	for (uint8_t i = 0; i < 67; ++i)
	{
		if (eflexpwm_pin_config[i].pin_name == pin_name)
		{
			module = (eflexpwm_pin_config[i].mod_sub_channel >> 5) & 0x7;
			sub_module = (eflexpwm_pin_config[i].mod_sub_channel >> 2) & 0x7;
			channel = (eflexpwm_pin_config[i].mod_sub_channel) & 0x3;
			config.mux_mode = eflexpwm_pin_config[i].mux_mode;
			config.force_input = false;
			return true;
		}
	}
	return false;
}

bool GetUartTXPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	for (uint8_t i = 0; i < 18; ++i)
	{
		if (uart_tx_pin_config[i].pin_name == pin)
		{
			module = uart_tx_pin_config[i].module;
			config.mux_mode = uart_tx_pin_config[i].mux_mode;
			if (uart_tx_pin_config[i].select_input_reg)
				*((volatile uint32_t *)(uart_tx_pin_config[i].select_input_reg)) = uart_tx_pin_config[i].select_input_value;
			config.pin_config.hysteresis_enable = false;
			config.pin_config.fast_slew_rate = false;
			config.pin_config.drive_strength = Config::PinConfig::DriveStrength::kDSE6;
			config.pin_config.speed = Config::PinConfig::Speed::k100MHz2;
			config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
			config.pin_config.open_drain_enable = false;
			config.force_input = false;
			return true;
		}
	}
	return false;
}

bool GetUartRXPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	for (uint8_t i = 0; i < 18; ++i)
	{
		if (uart_rx_pin_config[i].pin_name == pin)
		{
			module = uart_rx_pin_config[i].module;
			config.mux_mode = uart_rx_pin_config[i].mux_mode;
			if (uart_rx_pin_config[i].select_input_reg)
				*((volatile uint32_t *)(uart_rx_pin_config[i].select_input_reg)) = uart_rx_pin_config[i].select_input_value;
			config.pin_config.hysteresis_enable = false;
			config.pin_config.fast_slew_rate = false;
			config.pin_config.drive_strength = Config::PinConfig::DriveStrength::kDSE6;
			config.pin_config.speed = Config::PinConfig::Speed::k100MHz2;
			config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
			config.pin_config.open_drain_enable = false;
			config.force_input = false;
			return true;
		}
	}
	return false;
}

bool GetI2CSclPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	for (uint8_t i = 0; i < 10; ++i)
	{
		if (i2c_scl_pin_config[i].pin_name == pin)
		{
			module = i2c_scl_pin_config[i].module;
			config.mux_mode = i2c_scl_pin_config[i].mux_mode;
			*((volatile uint32_t *)(i2c_scl_pin_config[i].select_input_reg)) = i2c_scl_pin_config[i].select_input_value;
			config.pin_config.hysteresis_enable = false;
			config.pin_config.fast_slew_rate = false;
			config.pin_config.drive_strength = Config::PinConfig::DriveStrength::kDSE6;
			config.pin_config.speed = Config::PinConfig::Speed::k100MHz2;
			config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
			config.pin_config.open_drain_enable = true;
			config.force_input = true;
			return true;
		}
	}
	return false;
}

bool GetI2CSdaPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	for (uint8_t i = 0; i < 10; ++i)
	{
		if (i2c_sda_pin_config[i].pin_name == pin)
		{
			module = i2c_sda_pin_config[i].module;
			config.mux_mode = i2c_sda_pin_config[i].mux_mode;
			*((volatile uint32_t *)(i2c_sda_pin_config[i].select_input_reg)) = i2c_sda_pin_config[i].select_input_value;
			config.pin_config.hysteresis_enable = false;
			config.pin_config.fast_slew_rate = false;
			config.pin_config.drive_strength = Config::PinConfig::DriveStrength::kDSE6;
			config.pin_config.speed = Config::PinConfig::Speed::k100MHz2;
			config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
			config.pin_config.open_drain_enable = true;
			config.force_input = true;
			return true;
		}
	}
	return false;
}

bool GetSpiSckPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_27)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F84F0U) = 0;
		module = 1;
	}
	else if (pin == Name::kGPIO_SD_B0_00)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F84F0U) = 1;
		module = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_07)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F8500U) = 0;
		module = 2;
	}
	else if (pin == Name::kGPIO_EMC_00)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F8500U) = 1;
		module = 2;
	}
	else if (pin == Name::kGPIO_AD_B0_00)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		*((uint32_t *)0x401F8510U) = 0;
		module = 3;
	}
	else if (pin == Name::kGPIO_AD_B1_15)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F8510U) = 1;
		module = 3;
	}
	else if (pin == Name::kGPIO_B0_03)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F8520U) = 0;
		module = 4;
	}
	else if (pin == Name::kGPIO_B1_07)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F8520U) = 1;
		module = 4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetSpiSdoPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_28)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F84F8U) = 0;
		module = 1;
	}
	else if (pin == Name::kGPIO_SD_B0_02)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F84F8U) = 1;
		module = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_08)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F8508U) = 0;
		module = 2;
	}
	else if (pin == Name::kGPIO_EMC_02)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F8508U) = 1;
		module = 2;
	}
	else if (pin == Name::kGPIO_AD_B0_01)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		*((uint32_t *)0x401F8518U) = 0;
		module = 3;
	}
	else if (pin == Name::kGPIO_AD_B1_14)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F8518U) = 1;
		module = 3;
	}
	else if (pin == Name::kGPIO_B0_02)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F8528U) = 0;
		module = 4;
	}
	else if (pin == Name::kGPIO_B1_06)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F8528U) = 1;
		module = 4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetSpiSdiPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_29)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F84F4U) = 0;
		module = 1;
	}
	else if (pin == Name::kGPIO_SD_B0_03)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F84F4U) = 1;
		module = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_09)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F8504U) = 0;
		module = 2;
	}
	else if (pin == Name::kGPIO_EMC_03)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F8504U) = 1;
		module = 2;
	}
	else if (pin == Name::kGPIO_AD_B0_02)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		*((uint32_t *)0x401F8514U) = 0;
		module = 3;
	}
	else if (pin == Name::kGPIO_AD_B1_13)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F8514U) = 1;
		module = 3;
	}
	else if (pin == Name::kGPIO_B0_01)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F8524U) = 0;
		module = 4;
	}
	else if (pin == Name::kGPIO_B1_05)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F8524U) = 1;
		module = 4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetSpiPCSPinConfig(Config &config, uint8_t &module, uint8_t &cs)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_30)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F84ECU) = 1;
		module = 1;
		cs = 0;
	}
	else if (pin == Name::kGPIO_SD_B0_01)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F84ECU) = 0;
		module = 1;
		cs = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_06)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		*((uint32_t *)0x401F84FCU) = 0;
		module = 2;
		cs = 0;
	}
	else if (pin == Name::kGPIO_EMC_01)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F84FCU) = 1;
		module = 2;
		cs = 0;
	}
	else if (pin == Name::kGPIO_AD_B0_03)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		*((uint32_t *)0x401F850CU) = 0;
		module = 3;
		cs = 0;
	}
	else if (pin == Name::kGPIO_AD_B1_12)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		*((uint32_t *)0x401F850CU) = 1;
		module = 3;
		cs = 0;
	}
	else if (pin == Name::kGPIO_B0_00)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		*((uint32_t *)0x401F851CU) = 0;
		module = 4;
		cs = 0;
	}
	else if (pin == Name::kGPIO_B1_04)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F851CU) = 1;
		module = 4;
		cs = 0;
	}
	else if (pin == Name::kGPIO_EMC_31)
	{
		config.mux_mode = Config::MuxMode::kAlt3;
		module = 1;
		cs = 1;
	}
	else if (pin == Name::kGPIO_EMC_14)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		module = 2;
		cs = 1;
	}
	else if (pin == Name::kGPIO_AD_B0_04)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		module = 3;
		cs = 1;
	}
	else if (pin == Name::kGPIO_B1_03)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		module = 4;
		cs = 1;
	}
	else if (pin == Name::kGPIO_EMC_40)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		module = 1;
		cs = 2;
	}
	else if (pin == Name::kGPIO_SD_B1_10)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		module = 2;
		cs = 2;
	}
	else if (pin == Name::kGPIO_AD_B0_05)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		module = 3;
		cs = 2;
	}
	else if (pin == Name::kGPIO_B1_02)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		module = 4;
		cs = 2;
	}
	else if (pin == Name::kGPIO_EMC_41)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		module = 1;
		cs = 3;
	}
	else if (pin == Name::kGPIO_SD_B1_11)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		module = 2;
		cs = 3;
	}
	else if (pin == Name::kGPIO_AD_B0_06)
	{
		config.mux_mode = Config::MuxMode::kAlt7;
		module = 3;
		cs = 3;
	}
	else if (pin == Name::kGPIO_B1_11)
	{
		config.mux_mode = Config::MuxMode::kAlt6;
		module = 4;
		cs = 3;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetFlexSpiSckPinConfig(Config &config)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_SD_B1_07)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84C8U) = 0;
	}
	else if (pin == Name::kGPIO_AD_B1_14)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84C8U) = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_04)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F8504U) = 0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = true;
	return true;
}

bool GetFlexSpiSioPinConfig(Config &config)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B1_10)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84B4U) = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_11)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84B0U) = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_12)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84ACU) = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_13)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84A8U) = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_08)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84A8U) = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_09)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84ACU) = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_10)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84B0U) = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_11)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84B4U) = 0;
	}
	else if (pin == Name::kGPIO_AD_B1_04)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84C4U) = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_05)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84C0U) = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_06)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84BCU) = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_07)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84B8U) = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_00)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84C4U) = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_01)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84C0U) = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_02)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84BCU) = 0;
	}
	else if (pin == Name::kGPIO_SD_B1_03)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84B8U) = 0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = true;
	return true;
}

bool GetFlexSpiCSPinConfig(Config &config, uint8_t &cs)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B1_08)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		cs = 1;
	}
	else if (pin == Name::kGPIO_AD_B1_15)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		cs = 0;
	}
	else if (pin == Name::kGPIO_SD_B0_00)
	{
		config.mux_mode = Config::MuxMode::kAlt6;
		cs = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_04)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		cs = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_06)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		cs = 0;
	}
	else if (pin == Name::kGPIO_SD_B0_01)
	{
		config.mux_mode = Config::MuxMode::kAlt6;
		cs = 3;
	}
	else if (pin == Name::kGPIO_SD_B0_04)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		cs = 2;
	}
	else if (pin == Name::kGPIO_SD_B1_05)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
		cs = 2;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = true;
	return true;
}

bool GetFlexSpiDQSPinConfig(Config &config)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_SD_B0_05)
	{
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else if (pin == Name::kGPIO_AD_B1_09)
	{
		config.mux_mode = Config::MuxMode::kAlt0;
		*((uint32_t *)0x401F84A4U) = 1;
	}
	else if (pin == Name::kGPIO_SD_B1_05)
	{
		config.mux_mode = Config::MuxMode::kAlt1;
		*((uint32_t *)0x401F84A4U) = 0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = true;
	return true;
}

bool GetCSIData0PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_10)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		module = 0;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData1PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_11)
	{
		config.mux_mode = Config::MuxMode::kAlt2;
		module = 0;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData2PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_11 || pin == Name::kGPIO_AD_B1_15)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_11)
		{
			*((uint32_t *)0x401F8400U) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F8400U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData3PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_10 || pin == Name::kGPIO_AD_B1_14)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_10)
		{
			*((uint32_t *)0x401F8404U) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F8404U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData4PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_09 || pin == Name::kGPIO_AD_B1_13)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_09)
		{
			*((uint32_t *)0x401F8408U) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F8408U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData5PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_08 || pin == Name::kGPIO_AD_B1_12)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_08)
		{
			*((uint32_t *)0x401F840CU) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F840CU) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData6PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_07 || pin == Name::kGPIO_AD_B1_11)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_07)
		{
			*((uint32_t *)0x401F8410U) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F8410U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData7PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_06 || pin == Name::kGPIO_AD_B1_10)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_06)
		{
			*((uint32_t *)0x401F8414U) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F8414U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData8PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B1_09 || pin == Name::kGPIO_AD_B0_05)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_05)
		{
			*((uint32_t *)0x401F8418U) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F8418U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData9PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B1_08 || pin == Name::kGPIO_AD_B0_04)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B0_04)
		{
			*((uint32_t *)0x401F841CU) = 0x1u;
		}
		else
		{
			*((uint32_t *)0x401F841CU) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData10PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_09)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData11PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_08)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData12PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_07)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData13PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_06)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData14PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_05)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData15PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_04)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData16PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_37)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData17PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_36)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData18PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_35)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData19PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_34)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData20PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_33)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData21PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_32)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData22PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_31)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIData23PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_30)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIFieldPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_EMC_38)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIHsyncPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B1_07 || pin == Name::kGPIO_B1_14 || pin == Name::kGPIO_AD_B0_15)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B1_07)
		{
			*((uint32_t *)0x401F8420U) = 0x1u;
		}
		else if (pin == Name::kGPIO_B1_14)
		{
			*((uint32_t *)0x401F8420U) = 0x2u;
			config.mux_mode = Config::MuxMode::kAlt2;
		}
		else
		{
			*((uint32_t *)0x401F8420U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIMclkPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B1_05 || pin == Name::kGPIO_B1_15)
	{
		module = 0;
		config.mux_mode = pin == Name::kGPIO_AD_B1_05 ? Config::MuxMode::kAlt4 : Config::MuxMode::kAlt2;
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIPclkPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_12 || pin == Name::kGPIO_AD_B1_04)
	{
		module = 0;
		if (pin == Name::kGPIO_B1_12)
		{
			*((uint32_t *)0x401F8424U) = 0x1u;
			config.mux_mode = Config::MuxMode::kAlt2;
		}
		else
		{
			*((uint32_t *)0x401F8424U) = 0x0u;
			config.mux_mode = Config::MuxMode::kAlt4;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GetCSIVsyncPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_AD_B0_14 || pin == Name::kGPIO_AD_B1_06 || pin == Name::kGPIO_B1_13)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt4;
		if (pin == Name::kGPIO_AD_B1_06)
		{
			*((uint32_t *)0x401F8428U) = 0x1u;
		}
		else if (pin == Name::kGPIO_B1_13)
		{
			*((uint32_t *)0x401F8428U) = 0x2u;
			config.mux_mode = Config::MuxMode::kAlt2;
		}
		else
		{
			*((uint32_t *)0x401F8428U) = 0x0u;
		}
	}
	else
	{
		return false;
	}
	config.force_input = false;
	return true;
}

bool GeteLCDIFData0PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_04)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData1PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_05)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData2PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_06)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData3PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_07)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData4PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_08)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData5PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_09)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData6PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_10)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData7PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_11)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData8PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_12)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData9PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_13)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData10PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_14)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData11PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_15)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData12PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_00)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData13PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_01)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData14PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_02)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData15PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_03)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData16PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_04)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData17PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_05)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData18PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_06)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData19PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_07)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData20PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_08)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData21PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_09)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData22PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_10)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFData23PinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B1_11)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFEnablePinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_01)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFHsyncPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_02)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFVsyncPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_03)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GeteLCDIFPclkPinConfig(Config &config, uint8_t &module)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	if (pin == Name::kGPIO_B0_00)
	{
		module = 0;
		config.mux_mode = Config::MuxMode::kAlt0;
	}
	else
	{
		return false;
	}
	config.pin_config.fast_slew_rate = true;
	config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
	config.pin_config.open_drain_enable = false;
	config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
	config.pin_config.hysteresis_enable = false;
	config.force_input = false;
	return true;
}

bool GetXBARPinConfig(Config &config)
{
	if (pin_active[(uint8_t)config.pin])
	{
		return false;
	}
	Name pin = config.pin;
	for (uint8_t i = 0; i < 46; ++i)
	{
		if (pin == xbar_pin_config[i].pin_name)
		{
			config.mux_mode = xbar_pin_config[i].mux_mode;
			if (xbar_pin_config[i].select_input_reg)
			{
				*((uint32_t *)xbar_pin_config[i].select_input_reg) = xbar_pin_config[i].select_input_value;
			}
			config.pin_config.fast_slew_rate = true;
			config.pin_config.speed = Config::PinConfig::Speed::k200MHz;
			config.pin_config.open_drain_enable = false;
			config.pin_config.pull_keep_config = Config::PinConfig::PullKeepConfig::kKeep;
			config.pin_config.hysteresis_enable = false;
			config.force_input = true;
			return true;
		}
	}
	return false;
}

void InitPin(Config &config)
{
	if (pin_active[(uint8_t)config.pin])
	{
		assert(false);
	}
	else
	{
		pin_active[(uint8_t)config.pin] = true;
	}
	System::CLOCK_EnableClock(System::kCLOCK_Iomuxc); /* iomuxc clock (iomuxc_clk_enable): 0x03u */
	*((volatile uint32_t *)(0x401F8014U + ((uint32_t)config.pin) * 4u)) = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE((uint8_t)config.mux_mode) | IOMUXC_SW_MUX_CTL_PAD_SION(config.force_input);
	uint32_t pin_config_reg = 0;
	pin_config_reg |= (uint32_t)config.pin_config.fast_slew_rate;
	pin_config_reg |= (uint32_t)(((uint32_t)config.pin_config.drive_strength) << 3);
	pin_config_reg |= (uint32_t)(((uint32_t)config.pin_config.speed) << 6);
	pin_config_reg |= (uint32_t)(((uint32_t)config.pin_config.open_drain_enable) << 11);
	if (config.pin_config.pull_keep_config != Config::PinConfig::PullKeepConfig::kDisable)
	{
		pin_config_reg |= 1 << 12;
		if (config.pin_config.pull_keep_config == Config::PinConfig::PullKeepConfig::kPull)
		{
			pin_config_reg |= 1 << 13;
		}
		pin_config_reg |= (uint32_t)(((uint32_t)config.pin_config.pull_config) << 14);
	}
	pin_config_reg |= (uint32_t)(((uint32_t)config.pin_config.hysteresis_enable) << 16);
	*((volatile uint32_t *)(0x401F8204U + ((uint32_t)config.pin) * 4u)) = pin_config_reg;
}

void DeinitPin(System::Pinout::Name pin_name)
{
	pin_active[(uint8_t)pin_name] = false;
}

} // namespace Pinout
} // namespace System
