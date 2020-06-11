/*
 * xbar.cpp
 *
 *  Created on: 19 Dec 2018
 *      Author: lch19
 */

#include "driver/xbar.h"
#include "system/clock.h"
#include "system/cmsis/access_layer/access_layer.h"

namespace Driver
{

uint32_t XBARA::output_used[5] = {0, 0, 0, 0, 0x80000000};

void XBARA::Init()
{
	System::CLOCK_EnableClock(System::kCLOCK_Xbar1);
	output_used[3] = 0;
}

bool XBARA::OpenPin(System::Pinout::Name pin_name, XBAR1InputSignal &input_signal, XBAR1OutputSignal &output_signal)
{
	System::Pinout::Config pin_config;
	pin_config.pin = pin_name;
	if (System::Pinout::GetXBARPinConfig(pin_config))
	{
		System::Pinout::InitPin(pin_config);
		if (pin_name == System::Pinout::Name::kGPIO_EMC_00)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn02;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_01)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn03;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_02)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout04;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout04;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_03)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout05;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout05;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_04)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout06;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout06;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_05)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout07;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout07;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_06)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout08;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout08;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_07)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout09;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout09;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_08)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout17;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout17;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_12)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn24;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_13)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn25;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_14)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout19;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout19;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_15)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn20;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_16)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn21;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_35)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout18;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout18;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_36)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn22;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_EMC_37)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn23;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_00)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout14;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout14;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_01)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout15;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout15;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_02)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout16;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout16;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_03)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout17;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout17;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_05)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout17;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout17;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_06)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout18;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout18;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_07)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout19;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout19;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_08)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn20;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_09)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn21;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_10)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn22;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_11)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn23;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_14)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn24;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_AD_B0_15)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn25;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B0_12)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout10;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout10;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B0_13)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout11;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout11;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B0_14)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout12;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout12;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B0_15)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout13;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout13;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B1_00)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout14;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout14;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B1_01)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout15;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout15;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B1_02)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout16;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout16;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B1_03)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout17;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout17;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B1_14)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn02;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_B1_15)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarIn03;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_SD_B0_00)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout04;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout04;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_SD_B0_01)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout05;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout05;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_SD_B0_02)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout06;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout06;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_SD_B0_03)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout07;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout07;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_SD_B0_04)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout08;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout08;
		}
		else if (pin_name == System::Pinout::Name::kGPIO_SD_B0_05)
		{
			input_signal = XBAR1InputSignal::kIomuxXbarInout09;
			output_signal = XBAR1OutputSignal::kIomuxXbarInout09;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool XBARA::Connect(XBAR1InputSignal input_signal, XBAR1OutputSignal output_signal)
{
	if (output_used[4] >> 31)
	{
		Init();
	}
	uint8_t output = (uint16_t)output_signal & 0xFF;
	uint32_t *record = &(output_used[output >> 5]);
	if (((*record) >> (output % 32)) & 1)
	{
		return false;
	}
	else
	{
		*record |= 1 << (output % 32);
	}
	uint8_t input = (uint16_t)input_signal & 0xFF;
	volatile uint16_t *selx = (volatile uint16_t *)(((uintptr_t) & (XBARA1->SEL0)) + ((output >> 1) << 1));
	*selx = ((*selx & ~(0xFFU << (XBARA_SEL0_SEL1_SHIFT * ((output) % 2U)))) | ((input) << (XBARA_SEL0_SEL1_SHIFT * ((output) % 2U))));
	return true;
}

} // namespace Driver
