/*
 * MIMXRT1052.h
 *
 *  Created on: Aug 4, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_PINOUT_MIMXRT1052_H_
#define INC_PINOUT_MIMXRT1052_H_

#include "system/cmsis/access_layer/access_layer.h"

namespace System
{
namespace Pinout
{

enum struct Name
{
	kGPIO_EMC_00,
	kGPIO_EMC_01,
	kGPIO_EMC_02,
	kGPIO_EMC_03,
	kGPIO_EMC_04,
	kGPIO_EMC_05,
	kGPIO_EMC_06,
	kGPIO_EMC_07,
	kGPIO_EMC_08,
	kGPIO_EMC_09,
	kGPIO_EMC_10,
	kGPIO_EMC_11,
	kGPIO_EMC_12,
	kGPIO_EMC_13,
	kGPIO_EMC_14,
	kGPIO_EMC_15,
	kGPIO_EMC_16,
	kGPIO_EMC_17,
	kGPIO_EMC_18,
	kGPIO_EMC_19,
	kGPIO_EMC_20,
	kGPIO_EMC_21,
	kGPIO_EMC_22,
	kGPIO_EMC_23,
	kGPIO_EMC_24,
	kGPIO_EMC_25,
	kGPIO_EMC_26,
	kGPIO_EMC_27,
	kGPIO_EMC_28,
	kGPIO_EMC_29,
	kGPIO_EMC_30,
	kGPIO_EMC_31,
	kGPIO_EMC_32,
	kGPIO_EMC_33,
	kGPIO_EMC_34,
	kGPIO_EMC_35,
	kGPIO_EMC_36,
	kGPIO_EMC_37,
	kGPIO_EMC_38,
	kGPIO_EMC_39,
	kGPIO_EMC_40,
	kGPIO_EMC_41,
	kGPIO_AD_B0_00,
	kGPIO_AD_B0_01,
	kGPIO_AD_B0_02,
	kGPIO_AD_B0_03,
	kGPIO_AD_B0_04,
	kGPIO_AD_B0_05,
	kGPIO_AD_B0_06,
	kGPIO_AD_B0_07,
	kGPIO_AD_B0_08,
	kGPIO_AD_B0_09,
	kGPIO_AD_B0_10,
	kGPIO_AD_B0_11,
	kGPIO_AD_B0_12,
	kGPIO_AD_B0_13,
	kGPIO_AD_B0_14,
	kGPIO_AD_B0_15,
	kGPIO_AD_B1_00,
	kGPIO_AD_B1_01,
	kGPIO_AD_B1_02,
	kGPIO_AD_B1_03,
	kGPIO_AD_B1_04,
	kGPIO_AD_B1_05,
	kGPIO_AD_B1_06,
	kGPIO_AD_B1_07,
	kGPIO_AD_B1_08,
	kGPIO_AD_B1_09,
	kGPIO_AD_B1_10,
	kGPIO_AD_B1_11,
	kGPIO_AD_B1_12,
	kGPIO_AD_B1_13,
	kGPIO_AD_B1_14,
	kGPIO_AD_B1_15,
	kGPIO_B0_00,
	kGPIO_B0_01,
	kGPIO_B0_02,
	kGPIO_B0_03,
	kGPIO_B0_04,
	kGPIO_B0_05,
	kGPIO_B0_06,
	kGPIO_B0_07,
	kGPIO_B0_08,
	kGPIO_B0_09,
	kGPIO_B0_10,
	kGPIO_B0_11,
	kGPIO_B0_12,
	kGPIO_B0_13,
	kGPIO_B0_14,
	kGPIO_B0_15,
	kGPIO_B1_00,
	kGPIO_B1_01,
	kGPIO_B1_02,
	kGPIO_B1_03,
	kGPIO_B1_04,
	kGPIO_B1_05,
	kGPIO_B1_06,
	kGPIO_B1_07,
	kGPIO_B1_08,
	kGPIO_B1_09,
	kGPIO_B1_10,
	kGPIO_B1_11,
	kGPIO_B1_12,
	kGPIO_B1_13,
	kGPIO_B1_14,
	kGPIO_B1_15,
	kGPIO_SD_B0_00,
	kGPIO_SD_B0_01,
	kGPIO_SD_B0_02,
	kGPIO_SD_B0_03,
	kGPIO_SD_B0_04,
	kGPIO_SD_B0_05,
	kGPIO_SD_B1_00,
	kGPIO_SD_B1_01,
	kGPIO_SD_B1_02,
	kGPIO_SD_B1_03,
	kGPIO_SD_B1_04,
	kGPIO_SD_B1_05,
	kGPIO_SD_B1_06,
	kGPIO_SD_B1_07,
	kGPIO_SD_B1_08,
	kGPIO_SD_B1_09,
	kGPIO_SD_B1_10,
	kGPIO_SD_B1_11,
	kDisable
};

struct Config
{
	enum struct MuxMode
	{
		kAlt0,
		kAlt1,
		kAlt2,
		kAlt3,
		kAlt4,
		kAlt5,
		kAlt6,
		kAlt7
	};

	struct PinConfig
	{
		/*DSE1: 260 Ohm @3.3V, 150 Ohm @1.8V, 240 Ohm for DDR, ~0.012 Amp output current
		 *DSE2: DSE1 resistance / 2, DSE 1 current * 2
		 *DSE3: DSE1 resistance / 3, DSE 1 current * 3
		 *DSE4: DSE1 resistance / 4, DSE 1 current * 4
		 *DSE5: DSE1 resistance / 5, DSE 1 current * 5
		 *DSE6: DSE1 resistance / 6, DSE 1 current * 6
		 *DSE7: DSE1 resistance / 7, DSE 1 current * 7
		 */
		enum struct DriveStrength
		{
			kDisable,
			kDSE1,
			kDSE2,
			kDSE3,
			kDSE4,
			kDSE5,
			kDSE6,
			kDSE7
		};
		enum struct Speed
		{
			k50MHz,
			k100MHz1,
			k100MHz2,
			k200MHz
		};
		enum struct PullKeepConfig
		{
			kDisable,
			kPull,
			kKeep
		};
		/*
		 * 100kPullDown: 100k Ohm pull down
		 * 47kPullUp: 47k Ohm pull up
		 * 100kPullUp: 100k Ohm pull up
		 * 22kPullUp: 22k Ohm pull up
		 */
		enum struct PullConfig
		{
			k100kPullDown,
			k47kPullUp,
			k100kPullUp,
			k22kPullUp
		};
		bool fast_slew_rate = false;
		DriveStrength drive_strength = DriveStrength::kDSE6;
		Speed speed = Speed::k100MHz2;
		bool open_drain_enable = false;
		PullKeepConfig pull_keep_config = PullKeepConfig::kKeep;
		PullConfig pull_config = PullConfig::k22kPullUp; //Pull resistance select, require pull_keep_config set to kPull first
		bool hysteresis_enable = false;
	};
	Name pin;
	MuxMode mux_mode = MuxMode::kAlt0;
	PinConfig pin_config;
	bool force_input = false;
};

bool GetADCPinConfig(Config &config);

bool GeteFlexPWMPinConfig(Config &config, uint8_t &module, uint8_t &sub_module, uint8_t &channel);

bool GetUartTXPinConfig(Config &config, uint8_t &module);
bool GetUartRXPinConfig(Config &config, uint8_t &module);

bool GetI2CSclPinConfig(Config &config, uint8_t &module);
bool GetI2CSdaPinConfig(Config &config, uint8_t &module);

bool GetSpiSckPinConfig(Config &config, uint8_t &module);
bool GetSpiSdoPinConfig(Config &config, uint8_t &module);
bool GetSpiSdiPinConfig(Config &config, uint8_t &module);
bool GetSpiPCSPinConfig(Config &config, uint8_t &module, uint8_t &cs);

bool GetSpiSckPinConfig(Config &config, uint8_t &module);
bool GetSpiSdoPinConfig(Config &config, uint8_t &module);
bool GetSpiSdiPinConfig(Config &config, uint8_t &module);
bool GetSpiPCSPinConfig(Config &config, uint8_t &module, uint8_t &cs);

bool GetFlexSpiSckPinConfig(Config &config);
bool GetFlexSpiSioPinConfig(Config &config);
bool GetFlexSpiCSPinConfig(Config &config, uint8_t &cs);
bool GetFlexSpiDQSPinConfig(Config &config);

bool GetCSIData0PinConfig(Config &config, uint8_t &module);
bool GetCSIData1PinConfig(Config &config, uint8_t &module);
bool GetCSIData2PinConfig(Config &config, uint8_t &module);
bool GetCSIData3PinConfig(Config &config, uint8_t &module);
bool GetCSIData4PinConfig(Config &config, uint8_t &module);
bool GetCSIData5PinConfig(Config &config, uint8_t &module);
bool GetCSIData6PinConfig(Config &config, uint8_t &module);
bool GetCSIData7PinConfig(Config &config, uint8_t &module);
bool GetCSIData8PinConfig(Config &config, uint8_t &module);
bool GetCSIData9PinConfig(Config &config, uint8_t &module);
bool GetCSIData10PinConfig(Config &config, uint8_t &module);
bool GetCSIData11PinConfig(Config &config, uint8_t &module);
bool GetCSIData12PinConfig(Config &config, uint8_t &module);
bool GetCSIData13PinConfig(Config &config, uint8_t &module);
bool GetCSIData14PinConfig(Config &config, uint8_t &module);
bool GetCSIData15PinConfig(Config &config, uint8_t &module);
bool GetCSIData16PinConfig(Config &config, uint8_t &module);
bool GetCSIData17PinConfig(Config &config, uint8_t &module);
bool GetCSIData18PinConfig(Config &config, uint8_t &module);
bool GetCSIData19PinConfig(Config &config, uint8_t &module);
bool GetCSIData20PinConfig(Config &config, uint8_t &module);
bool GetCSIData21PinConfig(Config &config, uint8_t &module);
bool GetCSIData22PinConfig(Config &config, uint8_t &module);
bool GetCSIData23PinConfig(Config &config, uint8_t &module);
bool GetCSIFieldPinConfig(Config &config, uint8_t &module);
bool GetCSIHsyncPinConfig(Config &config, uint8_t &module);
bool GetCSIMclkPinConfig(Config &config, uint8_t &module);
bool GetCSIPclkPinConfig(Config &config, uint8_t &module);
bool GetCSIVsyncPinConfig(Config &config, uint8_t &module);

bool GeteLCDIFData0PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData1PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData2PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData3PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData4PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData5PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData6PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData7PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData8PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData9PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData10PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData11PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData12PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData13PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData14PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData15PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData16PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData17PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData18PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData19PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData20PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData21PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData22PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFData23PinConfig(Config &config, uint8_t &module);
bool GeteLCDIFEnablePinConfig(Config &config, uint8_t &module);
bool GeteLCDIFHsyncPinConfig(Config &config, uint8_t &module);
bool GeteLCDIFVsyncPinConfig(Config &config, uint8_t &module);
bool GeteLCDIFPclkPinConfig(Config &config, uint8_t &module);

bool GetXBARPinConfig(Config &config);

void InitPin(Config &config);
void DeinitPin(System::Pinout::Name pin_name);

} // namespace Pinout
} // namespace System

#endif /* INC_PINOUT_MIMXRT1052_H_ */
