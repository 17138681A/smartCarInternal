/*
 * hard_i2c_master.h
 *
 *  Created on: Oct 2, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_HARD_I2C_MASTER_H_
#define INC_DRIVER_HARD_I2C_MASTER_H_

#include "driver/i2c_master.h"
#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"

namespace Driver {

	/*!
	 * @ingroup i2c
	 * @{
	 */
	/**
	 * @brief Low Power Inter-Integrated Circuit
	 */
	class HardI2CMaster: public I2CMaster {
	public:
		HardI2CMaster(const I2CMaster::Config &config);

		/*
		 * @brief Send data to i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool SendByte(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t data);

		/*
		 * @brief Send data to i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool SendByte(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t data);

		/*
		 * @brief Send required number of data to i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool SendBytes(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t *data, uint8_t size);

		/*
		 * @brief Send required number of data to i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool SendBytes(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t *data, uint8_t size);

		/*
		 * @brief Get data from i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool GetByte(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t &data);

		/*
		 * @brief Get data to i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool GetByte(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t &data);

		/*
		 * @brief Get required number of data from i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool GetBytes(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t *data, uint8_t size);

		/*
		 * @brief Get required number of data from i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		bool GetBytes(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t *data, uint8_t size);

		bool SendByte(const uint8_t byte);
		bool SendByteWithStartCMD(const uint8_t byte);
		bool SendStopCMD();
		bool GetByte(uint8_t &byte);
		bool GetByte(uint8_t *byte, bool last_byte = true);

	private:
		inline void Reset() {
			i2c_base->MCR = LPI2C_MCR_RST_MASK;
			i2c_base->MCR = 0;
		}
		inline void Enable(bool enable) {
			i2c_base->MCR = (i2c_base->MCR & ~LPI2C_MCR_MEN_MASK) | LPI2C_MCR_MEN(enable);
		}
		inline void SetWatermarks(uint8_t tx_words, uint8_t rx_words) {
			i2c_base->MFCR = LPI2C_MFCR_TXWATER(tx_words) | LPI2C_MFCR_RXWATER(rx_words);
		}
		void SetBaudRate(uint32_t source_clock_Hz, uint32_t baud_rate_Hz);
		uint32_t GetCyclesForWidth(uint32_t source_clock_hz, uint32_t width_ns, uint32_t max_cycles, uint32_t prescaler);
		bool CheckForBusyBus();
		bool CheckAndClearError();
		bool WaitForTxReady();


		LPI2C_Type *i2c_base;
		uint32_t send_wait_time;
		uint32_t recieve_wait_time;
		System::Pinout::Name scl;
		System::Pinout::Name sda;
	};
/** @}*/

} // namespace Driver
#endif /* INC_DRIVER_HARD_I2C_MASTER_H_ */
