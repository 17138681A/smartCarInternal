/*
 * spi_master.h
 *
 *  Created on: Sep 17, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_SPI_MASTER_H_
#define INC_DRIVER_SPI_MASTER_H_

#include "system/pinout/pinout.h"
#include "system/cmsis/access_layer/access_layer.h"
#include <cstddef>

namespace Driver
{

/*!
 * @addtogroup lpspi Low Power Serial Peripheral Interface
 * @ingroup spi
 * The LPSPI is a low power Serial Peripheral Interface (SPI) module that supports an efficient interface to an SPI bus as a master and/or a slave. 
 * The LPSPI can continue operating in stop modes provided an appropriate clock is available and is designed for low CPU overhead with DMA offloading of FIFO register accesses.
 * @{
 */
/**
 * @brief Low Power Serial Peripheral Interface
 */
class SpiMaster
{
  public:
	struct Config
	{
		enum struct ClockPhase
		{
			kFirstEdge = 0U, /*!< CPHA=0. Data is captured on the leading edge of the SCK and changed on the
			 following edge.*/
			kSecondEdge = 1U /*!< CPHA=1. Data is changed on the leading edge of the SCK and captured on the
			 following edge.*/
		};
		enum struct PinConfig
		{
			kSdiInSdoOut,
			kSdiInSdiOut,
			kSdoInSdoOut,
			kSdoInSdiOut
		};
		enum struct DataOutConfig
		{
			kDataOutRetained = 0U, /*!< Data out retains last value when chip select is de-asserted */
			kDataOutTristate = 1U  /*!< Data out is tristated when chip select is de-asserted */
		};
		System::Pinout::Name sck;
		System::Pinout::Name sdo = System::Pinout::Name::kDisable;
		System::Pinout::Name sdi = System::Pinout::Name::kDisable;
		System::Pinout::Name cs;
		uint32_t baud_rate = 500000;					 /*!< Baud Rate for LPSPI. */
		uint32_t bits_per_frame = 8;					 /*!< Bits per frame, minimum 8, maximum 4096.*/
		bool clock_polarity_active_low = false;			 /*!< Clock polarity. */
		ClockPhase clock_phase = ClockPhase::kFirstEdge; /*!< Clock phase. */
		bool transfer_LSB_first = false;				 /*!< MSB or LSB data shift direction. */

		uint32_t pcs_to_sck_delay_ns = 1000000000 / baud_rate * 2;		 /*!< PCS to SCK delay time in nanoseconds, setting to 0 sets the minimum delay.
		 It sets the boundary value if out of range.*/
		uint32_t last_sck_to_pcs_delay_ns = 1000000000 / baud_rate * 2;  /*!< Last SCK to PCS delay time in nanoseconds, setting to 0 sets the minimum
		 delay. It sets the boundary value if out of range.*/
		uint32_t between_transfer_delay_ns = 1000000000 / baud_rate * 2; /*!< After the SCK delay time with nanoseconds, setting to 0 sets the minimum
		 delay. It sets the boundary value if out of range.*/
		bool chip_select_active_high = false;							 /*!< Desired PCS active high or low */
		PinConfig pin_config = PinConfig::kSdiInSdoOut;					 /*!< Configures which pins are used for input and output data during single bit transfers.*/
		DataOutConfig data_out_config = DataOutConfig::kDataOutRetained; /*!< Configures if the output data is tristated between accesses (LPSPI_PCS is negated). */
	};
	SpiMaster(const Config &config);
	inline void SendData(uint32_t data)
	{
		while (GetTxFifoCount() >= (uint32_t)(tx_fifo_size - 2));
		if (spi_base->TCR != (tcr | LPSPI_TCR_RXMSK(1)))
		{
			// when tcr is changed, the first two bytes tend to clash together
			// by setting txmask, an empty byte is sent while changing
			// this literally makes the byte a 16 bit data
			// but at least it works lol

			spi_base->TCR = (tcr | LPSPI_TCR_RXMSK(1) | LPSPI_TCR_TXMSK(1));
			while (!GetTransferState())
				;
			ClearTransferState();
		}
		spi_base->TDR = data;
	}
	inline void GetData(uint32_t &data)
	{
		while (GetTxFifoCount() >= (uint32_t)(tx_fifo_size - 1))
			;
		if (spi_base->TCR != tcr)
		{
			while (GetTxFifoCount())
				;
			spi_base->TCR = tcr;
			while (GetTxFifoCount())
				;
		}
		spi_base->TDR = 0;
		while (!GetRxFifoCount())
			;
		data = spi_base->RDR;
	}
	struct Transfer
	{
		const uint32_t *tx_data = nullptr;
		uint32_t *rx_data = nullptr;
		size_t tx_data_size = 0;
		size_t rx_data_size = 0;

		bool continuousTransfer = false;
	};
	void TransferBlocking(const Transfer &transfer);
	inline bool GetTransferState()
	{
		return (((spi_base->SR) >> 10) & 1);
	}
	inline void ClearTransferState()
	{
		spi_base->SR |= 0x3F00;
	}
	inline uint32_t GetTxFifoCount()
	{
		return ((spi_base->FSR & LPSPI_FSR_TXCOUNT_MASK) >> LPSPI_FSR_TXCOUNT_SHIFT);
	}
	inline uint32_t GetRxFifoCount()
	{
		return ((spi_base->FSR & LPSPI_FSR_RXCOUNT_MASK) >> LPSPI_FSR_RXCOUNT_SHIFT);
	}

  private:
	void Reset();
	void SetMaster();
	void SetOnePcsPolarity(uint8_t chip_select, bool chip_select_active_high);
	uint32_t SetBaudRate(uint32_t baud_rate, bool module_inited);
	void SetFifoWatermarks(uint8_t tx_watermark, uint8_t rx_watermark);
	void Enable(bool enable);
	void SetDelayTimes(uint32_t delay_ns, uint8_t type);
	void SetDelayScaler(uint32_t scaler, uint8_t type);

	LPSPI_Type *spi_base;
	uint8_t rx_fifo_size;
	uint8_t tx_fifo_size;
	uint8_t chip_select;
	uint8_t prescaler;
	uint32_t tcr;

	static uint8_t enabled_module;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_SPI_MASTER_H_ */

/*!
 * @defgroup spi Serial Peripheral Interface
 * @ingroup driver
 * @{
 */
/** @}*/
