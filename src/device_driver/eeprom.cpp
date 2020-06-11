/*
 * eeprom.cpp
 *
 *  Created on: 1 May 2019
 *      Author: manchak
 */

#include "device_driver/eeprom.h"
#include "system/pinout/pinout.h"

namespace DeviceDriver
{
EEPROM::EEPROM(const Config config) : i2c_master(config.i2c_master ? config.i2c_master : new Driver::SoftI2CMaster(GetI2CConfig())), DEVICE_ADDRESS(0b10100000)
{
}

const Driver::I2CMaster::Config EEPROM::GetI2CConfig()
{
	Driver::I2CMaster::Config config;
	config.scl = System::Pinout::Name::kGPIO_AD_B1_00;
	config.sda = System::Pinout::Name::kGPIO_AD_B1_01;
	config.baud_rate_Hz = 100000;
	return config;
}

bool EEPROM::readByte(uint8_t addr, uint8_t *dat)
{
	bool state = true;

	// dummy write address
	state &= i2c_master->SendByteWithStartCMD(DEVICE_ADDRESS);
	state &= i2c_master->SendByte(addr);

	// read data
	state &= i2c_master->SendByteWithStartCMD(DEVICE_ADDRESS | 0x01);
	state &= i2c_master->GetByte(dat, true);

	state &= i2c_master->SendStopCMD();

	System::Systick::DelayMS(100);
	return state;
}
bool EEPROM::readBytes(uint8_t addr, uint8_t *dat, uint16_t len)
{
	if (addr + len > 256)
		return false;

	bool state = true;

	// dummy write address
	state &= i2c_master->SendByteWithStartCMD(DEVICE_ADDRESS);
	state &= i2c_master->SendByte(addr);

	// read data
	state &= i2c_master->SendByteWithStartCMD(DEVICE_ADDRESS | 0x01);
	for (int i = 0; i < len - 1; ++i)
	{
		state &= i2c_master->GetByte((dat + i), false);
	}
	state &= i2c_master->GetByte((dat + len - 1), true);
	state &= i2c_master->SendStopCMD();

	System::Systick::DelayMS(100);
	return state;
}
bool EEPROM::writeByte(uint8_t addr, uint8_t *dat)
{
	bool state = true;
	state &= i2c_master->SendByte(DEVICE_ADDRESS >> 1, addr, *dat);
	System::Systick::DelayMS(100);
	return state;
}
bool EEPROM::writePage(uint8_t addr, uint8_t *dat, uint16_t len)
{
	if (((addr % 8) + len) > 8 || len == 0)
		return false;

	bool state = true;
	state &= i2c_master->SendBytes(DEVICE_ADDRESS >> 1, addr, dat, len);
	System::Systick::DelayMS(100);
	return state;
}

} // namespace DeviceDriver
