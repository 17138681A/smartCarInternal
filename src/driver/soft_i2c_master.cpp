/*
 * soft_i2c_master.cpp
 *
 *  Created on: Sep 27, 2018
 *      Author: LeeChunHei
 */

#include "driver/soft_i2c_master.h"
#include "system/systick.h"

namespace Driver
{

GPIO::Config GetPinConfig(const System::Pinout::Name pin)
{
	GPIO::Config config;
	config.pin = pin;
	config.gpio_dir = GPIO::Direction::kDigitalOutput;
	config.pin_config.open_drain_enable = true;
	config.force_input = true;
	config.default_high = true;
	return config;
}

SoftI2CMaster::SoftI2CMaster(const I2CMaster::Config &config) : scl(GetPinConfig(config.scl)), sda(GetPinConfig(config.sda)), wait_time_ns((1000000000 / config.baud_rate_Hz) >> 2)
{
}

void SoftI2CMaster::SendStartCMD()
{
	scl.Set(false);
	System::Systick::DelayNS(wait_time_ns);
	sda.Set(true);
	System::Systick::DelayNS(wait_time_ns);
	scl.Set(true);
	System::Systick::DelayNS(wait_time_ns);
	sda.Set(false);
	System::Systick::DelayNS(wait_time_ns);

	// scl.Set(true);
	// System::Systick::DelayNS(wait_time_ns);
	// sda.Set(false);
	// System::Systick::DelayNS(wait_time_ns);
	// scl.Set(false);
	// System::Systick::DelayNS(wait_time_ns);
	// sda.Set(true);
	// System::Systick::DelayNS(wait_time_ns);
}

bool SoftI2CMaster::SendStopCMD()
{
	
	scl.Set(false);
	System::Systick::DelayNS(wait_time_ns);
	sda.Set(false);
	System::Systick::DelayNS(wait_time_ns);
	scl.Set(true);
	System::Systick::DelayNS(wait_time_ns);
	sda.Set(true);
	System::Systick::DelayNS(wait_time_ns);

	return true;
}

bool SoftI2CMaster::SendByteWithStartCMD(const uint8_t byte) {
	SendStartCMD();
	return SendByte(byte);
}

bool SoftI2CMaster::SendByte(const uint8_t byte)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		scl.Set(false);
		System::Systick::DelayNS(wait_time_ns);
		if ((byte >> (7 - i)) & 1)
		{
			sda.Set(true);
		}
		else
		{
			sda.Set(false);
		}
		System::Systick::DelayNS(wait_time_ns);
		scl.Set(true);
		System::Systick::DelayNS(wait_time_ns << 1);
	}
	scl.Set(false);
	System::Systick::DelayNS(wait_time_ns);
	sda.Set(true);
	System::Systick::DelayNS(wait_time_ns);
	scl.Set(true);
	System::Systick::DelayNS(wait_time_ns);
	bool result = sda.Get();
	System::Systick::DelayNS(wait_time_ns);
	return !result;
}

bool SoftI2CMaster::GetByte(uint8_t &byte) {
	GetByte(&byte, true);
	return true;
}

bool SoftI2CMaster::GetByte(uint8_t *byte, bool last_byte)
{
	*byte = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		scl.Set(false);
		System::Systick::DelayNS(wait_time_ns);
		sda.Set(true);
		System::Systick::DelayNS(wait_time_ns);
		scl.Set(true);
		System::Systick::DelayNS(wait_time_ns);
		*byte |= sda.Get() << (7 - i);
		System::Systick::DelayNS(wait_time_ns);
	}
	scl.Set(false);
	System::Systick::DelayNS(wait_time_ns);
	sda.Set(last_byte);
	System::Systick::DelayNS(wait_time_ns);
	scl.Set(true);
	System::Systick::DelayNS(wait_time_ns << 1);

	return scl.Get(); // see if the slave would hold the clock line low
}

bool SoftI2CMaster::SendByte(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t data)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr) || !SendByte(data))
	{
		return false;
	}
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::SendByte(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t data)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr >> 8) || !SendByte(reg_addr & 0xFF) || !SendByte(data))
	{
		return false;
	}
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::SendBytes(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t *data, uint8_t size)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr))
	{
		return false;
	}
	for (uint8_t count = 0; count < size; ++count)
	{
		if (!SendByte(*(data + count)))
		{
			return false;
		}
	}
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::SendBytes(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t *data, uint8_t size)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr >> 8) || !SendByte(reg_addr & 0xFF))
	{
		return false;
	}
	for (uint8_t count = 0; count < size; ++count)
	{
		if (!SendByte(*(data + count)))
		{
			return false;
		}
	}
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::GetByte(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t &data)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr))
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte((slave_addr << 1) | 1))
	{
		return false;
	}
	GetByte(&data);
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::GetByte(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t &data)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr >> 8) || !SendByte(reg_addr & 0xFF))
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte((slave_addr << 1) | 1))
	{
		return false;
	}
	GetByte(&data);
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::GetBytes(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t *data, uint8_t size)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr))
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte((slave_addr << 1) | 1))
	{
		return false;
	}
	//	data = new uint8_t[size];
	for (uint8_t i = 0; i < size; i++)
	{
		data[i] = 0;
		GetByte(data + i, i == size - 1);
	}
	SendStopCMD();
	return true;
}

bool SoftI2CMaster::GetBytes(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t *data, uint8_t size)
{
	if (!scl.Get() || !sda.Get())
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte(slave_addr << 1) || !SendByte(reg_addr >> 8) || !SendByte(reg_addr & 0xFF))
	{
		return false;
	}
	SendStartCMD();
	if (!SendByte((slave_addr << 1) | 1))
	{
		return false;
	}
	//	data = new uint8_t[size];
	for (uint8_t i = 0; i < size; i++)
	{
		data[i] = 0;
		GetByte(data + i, i == size - 1);
	}
	SendStopCMD();
	return true;
}

} // namespace Driver
