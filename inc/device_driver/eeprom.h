/*
 * eeprom.h
 *
 *      Author: yimyim
 */

#ifndef DEVICE_DRIVER_EEPROM_H_
#define DEVICE_DRIVER_EEPROM_H_

#include "driver/i2c_master.h"
#include "driver/soft_i2c_master.h"
#include "driver/hard_i2c_master.h"
#include "system/systick.h"

/*
	for reading, if the cursor is beyond 256, it will transit data from the beginning
	for page write, each len must be less than 8, and so do (addr % 8) + len
 */

namespace DeviceDriver {

	class EEPROM {
	public:
		struct Config {
			Driver::I2CMaster *i2c_master = nullptr;
		};

		EEPROM(const Config);

		bool readByte(uint8_t addr, uint8_t *dat);
		bool readBytes(uint8_t addr, uint8_t *dat, uint16_t len);
		bool writeByte(uint8_t addr, uint8_t *dat);
		bool writePage(uint8_t addr, uint8_t *dat, uint16_t len);
	private:
		static const Driver::I2CMaster::Config GetI2CConfig();
		Driver::I2CMaster* i2c_master;
		const uint8_t DEVICE_ADDRESS;
	};
}
#endif
