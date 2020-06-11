/*
 * uart.h
 *
 *  Created on: Aug 26, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_UART_H_
#define INC_DRIVER_UART_H_

#include <stdint.h>
#include <functional>
#include <vector>
#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"

namespace Driver
{

/*!
 * @addtogroup uart Universal Asynchronous Receiver/Transmitter
 * @ingroup driver
 * @{
 */
/**
 * @brief Universal Asynchronous Receiver/Transmitter
 */
class Uart
{
  public:
	typedef std::function<void(std::vector<uint8_t> &data)> Uart_Listener;
	struct Config
	{
		enum struct ParityMode
		{
			kDisableParity = 0x0U, //No parity bit
			kEvenParity = 0x2U,	//Even parity bit
			kOddParity = 0x3U	  //Odd parity bit
		};
		enum struct DataBits
		{
			kEightDataBits, //8 bits data
			kSevenDataBits  //7 bits data
		};
		enum struct StopBit
		{
			kOneStopBit,
			kTwoStopBit
		};
		enum struct TransmitCTSSource
		{
			kCtsSourcePin,
			kCtsSourceMatchResult
		};
		enum struct TransmitCTSConfig
		{
			kCtsSampleAtStart,
			kCtsSampleAtIdle
		};

		/*! @brief LPUART idle flag type defines when the receiver starts counting. */
		enum struct IdleType
		{
			kIdleTypeStartBit,
			kIdleTypeStopBit
		};

		/*! @brief LPUART idle detected configuration.
		 *  This structure defines the number of idle characters that must be received before
		 *  the IDLE flag is set.
		 */
		enum struct IdleConfig
		{
			kLPUART_IdleCharacter1,
			kLPUART_IdleCharacter2,
			kLPUART_IdleCharacter4,
			kLPUART_IdleCharacter8,
			kLPUART_IdleCharacter16,
			kLPUART_IdleCharacter32,
			kLPUART_IdleCharacter64,
			kLPUART_IdleCharacter128
		};

		System::Pinout::Name tx_pin = System::Pinout::Name::kDisable;			//TX pin
		System::Pinout::Name rx_pin = System::Pinout::Name::kDisable;			//RX pin
		uint32_t baud_rate = 115200;											//Baud rate
		ParityMode parity_mode = ParityMode::kDisableParity;					//Parity mode
		DataBits data_bits = DataBits::kEightDataBits;							//Data bits count
		bool is_MSB = false;													//Data bits order, true->MSB, false->LSB
		StopBit stop_bit = StopBit::kOneStopBit;								//Number of stop bit
		uint8_t tx_fifo_watermark = 0;											//TX FIFO watermark, must not larger than 4
		uint8_t rx_fifo_watermark = 0;											//RX FIFO watermark, must not larger than 4
		bool enable_rx_rts = false;												//Enable RX RTS
		bool enable_tx_cts = false;												//Enable TX CTS
		TransmitCTSSource tx_cts_source = TransmitCTSSource::kCtsSourcePin;		//TX CTS source
		TransmitCTSConfig tx_cts_config = TransmitCTSConfig::kCtsSampleAtStart; //TX CTS configure
		IdleType rx_idle_type = IdleType::kIdleTypeStartBit;					//RX IDLE type.
		IdleConfig rx_idle_config = IdleConfig::kLPUART_IdleCharacter1;			//RX IDLE configuration.
		Uart_Listener tx_empty_listener = nullptr;
		Uart_Listener rx_full_listener = nullptr;
		uint8_t interrupt_priority = 15;
	};
	Uart(const Config &config);
	inline void SendByte(const uint8_t &data)
	{
		uart_base->DATA = data;
		if (had_tx_listener)
			uart_base->CTRL |= LPUART_CTRL_TCIE_MASK;
	}
	inline uint8_t ReadByte()
	{
		return (bool)m_data_bits ? (uart_base->DATA & 0x7F) : uart_base->DATA;
	}
	void SendByteBuffer(const uint8_t *data, uint32_t length);
	inline void SetTXEmptyInterrupt(bool enable){
		if(enable){
			uart_base->CTRL |= LPUART_CTRL_TCIE_MASK;
		}else{
			uart_base->CTRL &= ~LPUART_CTRL_TCIE_MASK;
		}
	}
	inline void SetRXFUllInterrupt(bool enable){
		if(enable){
			uart_base->CTRL |= LPUART_CTRL_RIE_MASK;
		}else{
			uart_base->CTRL &= ~LPUART_CTRL_RIE_MASK;
		}
	}

  private:
	LPUART_Type *uart_base;
	Config::DataBits m_data_bits;
	bool had_tx_listener;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_UART_H_ */
