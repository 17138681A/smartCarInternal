/*
 * dma.h
 *
 *  Created on: Aug 31, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_DMA_H_
#define INC_DRIVER_DMA_H_

#include "system/cmsis/access_layer/access_layer.h"
#include <functional>

namespace Driver
{

/*!
 * @addtogroup dma Direct Memory Access
 * @ingroup driver
 * The direct memory access controller is a module capable of performing complex data transfers with minimal intervention from a host processor
 * @{
 */
/**
 * @brief Direct Memory Access
 */
class DMA
{
  public:
	typedef std::function<void(DMA *)> DMA_Listener;

	struct Config
	{
		bool enable_continuous_link_mode = false;	/*!< Enable (true) continuous link mode. Upon minor loop completion, the channel
		 activates again if that channel has a minor loop channel link enabled and
		 the link channel is itself. */
		bool enable_halt_on_error = true;			 /*!< Enable (true) transfer halt on error. Any error causes the HALT bit to set.
		 Subsequently, all service requests are ignored until the HALT bit is cleared.*/
		bool enable_round_robin_arbitration = false; /*!< Enable (true) round robin channel arbitration method or fixed priority
		 arbitration is used for channel selection */
		bool enable_debug_mode = false;				 /*!< Enable(true) eDMA debug mode. When in debug mode, the eDMA stalls the start of
		 a new channel. Executing channels are allowed to complete. */
	};
	DMA(const Config &config);

	struct TCDConfig
	{
		uint32_t SADDR;		/*!< SADDR register, used to save source address */
		uint16_t SOFF;		/*!< SOFF register, save offset bytes every transfer */
		uint16_t ATTR;		/*!< ATTR register, source/destination transfer size and modulo */
		uint32_t NBYTES;	/*!< Nbytes register, minor loop length in bytes */
		uint32_t SLAST;		/*!< SLAST register */
		uint32_t DADDR;		/*!< DADDR register, used for destination address */
		uint16_t DOFF;		/*!< DOFF register, used for destination offset */
		uint16_t CITER;		/*!< CITER register, current minor loop numbers, for unfinished minor loop.*/
		uint32_t DLAST_SGA; /*!< DLASTSGA register, next stcd address used in scatter-gather mode */
		uint16_t CSR;		/*!< CSR register, for TCD control status */
		uint16_t BITER;		/*!< BITER register, begin minor loop count. */
	};

	struct ChannelConfig
	{
		uint32_t channel;
		DMA_Listener listener = nullptr;
		uint8_t interrupt_priority = 15; //Interrupt priority, range [0-15], smaller value means higher priority
	};
	void ConfigChannel(const ChannelConfig &config);

	struct TransferConfig
	{
		enum struct TransferSize
		{
			k1Bytes = 0x0U,  /*!< Source/Destination data transfer size is 1 byte every time */
			k2Bytes = 0x1U,  /*!< Source/Destination data transfer size is 2 bytes every time */
			k4Bytes = 0x2U,  /*!< Source/Destination data transfer size is 4 bytes every time */
			k8Bytes = 0x3U,  /*!< Source/Destination data transfer size is 8 bytes every time */
			k16Bytes = 0x4U, /*!< Source/Destination data transfer size is 16 bytes every time */
			k32Bytes = 0x5U, /*!< Source/Destination data transfer size is 32 bytes every time */
		};
		uint32_t src_addr;				 /*!< Source data address. */
		uint32_t dest_addr;				 /*!< Destination data address. */
		TransferSize src_transfer_size;  /*!< Source data transfer size. */
		TransferSize dest_transfer_size; /*!< Destination data transfer size. */
		int16_t src_offset;				 /*!< Sign-extended offset applied to the current source address to
		 form the next-state value as each source read is completed. */
		int16_t dest_offset;			 /*!< Sign-extended offset applied to the current destination address to
		 form the next-state value as each destination write is completed. */
		uint32_t minor_loop_bytes;		 /*!< Bytes to transfer in a minor loop*/
		uint32_t major_loop_counts;		 /*!< Major loop iteration count. */
		uint32_t channel;
	};
	bool ConfigTransfer(const TransferConfig &config);
	void StartTransfer(const uint32_t channel);
	inline uint16_t GetDMACount(const uint32_t channel)
	{
		return dma_base->TCD[channel].CITER_ELINKNO;
	}

  private:
	DMA_Type *dma_base;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_DMA_H_ */
