/*
 * i2c_master.h
 *
 *  Created on: Sep 4, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_I2C_MASTER_H_
#define INC_DRIVER_I2C_MASTER_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"

namespace Driver {

	/*!
	 * @addtogroup i2c Inter-Integrated Circuit
	 * @ingroup driver
	 * Inter-Integrated Circuit(I2C) is a multi-master, multi-slave, packet switched, single-ended, serial computer bus.
	 * It is typically used for attaching lower-speed peripheral ICs to processors and microcontrollers in short-distance, intra-board communication.
	 * @{
	 */
	/**
	 * @brief Protocol for Inter-Integrated Circuit library
	 */
	class I2CMaster {
	public:
		struct Config {
			enum struct PinConfig {
				k2PinOpenDrain, /*!< LPI2C Configured for 2-pin open drain mode */
				k2PinOutputOnly, /*!< LPI2C Configured for 2-pin output only mode (ultra-fast mode) */
				k2PinPushPull, /*!< LPI2C Configured for 2-pin push-pull mode */
				k4PinPushPull, /*!< LPI2C Configured for 4-pin push-pull mode */
				k2PinOpenDrainWithSeparateSlave, /*!< LPI2C Configured for 2-pin open drain mode with separate LPI2C slave */
				k2PinOutputOnlyWithSeparateSlave, /*!< LPI2C Configured for 2-pin output only mode(ultra-fast mode) with separate LPI2C slave */
				k2PinPushPullWithSeparateSlave, /*!< LPI2C Configured for 2-pin push-pull mode with separate LPI2C slave */
				k4PinPushPullWithInvertedOutput /*!< LPI2C Configured for 4-pin push-pull mode(inverted outputs) */
			};
			struct HostRequest {
				enum struct Source {
					kExternalPin, /*!< Select the LPI2C_HREQ pin as the host request input */
					kInputTrigger /*!< Select the input trigger as the host request input */
				};
				bool enable = false; /*!< Enable host request. */
				Source source = Source::kExternalPin; /*!< Host request source. */
				bool is_active_high = true; /*!< Host request pin polarity. */
			};
			System::Pinout::Name scl;
			System::Pinout::Name sda;
			bool enable_master = true; /*!< Whether to enable master mode. */
			bool enable_doze = true; /*!< Whether master is enabled in doze mode. */
			bool debug_enable = false; /*!< Enable transfers to continue when halted in debug mode. */
			bool ignore_ack = false; /*!< Whether to ignore ACK/NACK. */
			PinConfig pin_config = PinConfig::k2PinOpenDrain; /*!< The pin configuration option. */
			uint32_t baud_rate_Hz = 100000u; /*!< Desired baud rate in Hertz. */
			uint32_t bus_idle_timeout_ns = 0; /*!< Bus idle timeout in nanoseconds. Set to 0 to disable. */
			uint32_t pin_low_timeout_ns = 0; /*!< Pin low timeout in nanoseconds. Set to 0 to disable. */
			uint8_t sda_glitch_filter_width_ns = 0; /*!< Width in nanoseconds of glitch filter on SDA pin. Set to 0 to disable. */
			uint8_t scl_glitch_filter_width_ns = 0; /*!< Width in nanoseconds of glitch filter on SCL pin. Set to 0 to disable. */
			uint32_t send_wait_time = 0;	//The time in ns that i2c will wait for the tx fifo is empty, default 0 means no waiting time given
			uint32_t recieve_wait_time = 0;	//The time in ns that i2c will wait for the data come in, default 0 means no waiting time given
			HostRequest host_request;/*!< Host request options. */
		};

		/*
		 * @brief Send data to i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool SendByte(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t data)=0;

		/*
		 * @brief Send data to i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool SendByte(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t data)=0;

		/*
		 * @brief Send required number of data to i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool SendBytes(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t* data, uint8_t size)=0;

		/*
		 * @brief Send required number of data to i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: data to send, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool SendBytes(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t* data, uint8_t size)=0;

		/*
		 * @brief Get data from i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool GetByte(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t& data)=0;

		/*
		 * @brief Get data to i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool GetByte(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t& data)=0;

		/*
		 * @brief Get required number of data from i2c device with corresponding slave address and 8-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool GetBytes(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t* data, uint8_t size)=0;

		/*
		 * @brief Get required number of data from i2c device with corresponding slave address and 16-bit register address
		 *
		 * @param slave_addr: i2c device slave address, reg_addr: target register address, data: received data, size: required data number.
		 *
		 * @retval status of transmission, true when transfer success.
		 */
		virtual bool GetBytes(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t* data, uint8_t size)=0;

		/*
		 *  send one byte out
		 */

		virtual bool SendByte(const uint8_t byte)=0;
		virtual bool SendByteWithStartCMD(const uint8_t byte)=0;
		virtual bool SendStopCMD()=0;
		virtual bool GetByte(uint8_t &byte)=0;
		virtual bool GetByte(uint8_t *byte, bool last_byte = true)=0;
	};
/** @}*/

}

#endif /* INC_DRIVER_I2C_MASTER_H_ */
