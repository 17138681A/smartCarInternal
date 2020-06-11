/*
 * csi.h
 *
 *  Created on: Sep 3, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_CSI_H_
#define INC_DRIVER_CSI_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"
#include <functional>

namespace Driver
{

/*!
 * @addtogroup csi CMOS Sensor Interface
 * @ingroup driver
 * The CSI enables the chip to connect directly to external CMOS image sensors
 * @{
 */
/**
 * @brief CMOS Sensor Interface
 */
class Csi
{
  public:
	typedef std::function<void(Csi *)> Csi_Listener;
	struct Config
	{
		enum struct WorkMode
		{
			kGatedClockMode = CSI_CSICR1_GCLK_MODE(1U),		  /*!< HSYNC, VSYNC, and PIXCLK signals are used. */
			kNonGatedClockMode = 0U,						  /*!< VSYNC, and PIXCLK signals are used. */
			kCCIR656ProgressiveMode = CSI_CSICR1_CCIR_EN(1U), /*!< CCIR656 progressive mode. */
		};
		enum struct DataBus
		{
			k8Bit, /*!< 8-bit data bus. */
		};
		struct PinList
		{
			System::Pinout::Name pclk = System::Pinout::Name::kDisable;
			System::Pinout::Name mclk = System::Pinout::Name::kDisable;
			System::Pinout::Name vsync = System::Pinout::Name::kDisable;
			System::Pinout::Name hsync = System::Pinout::Name::kDisable;
			System::Pinout::Name field = System::Pinout::Name::kDisable;
			System::Pinout::Name data0 = System::Pinout::Name::kDisable;
			System::Pinout::Name data1 = System::Pinout::Name::kDisable;
			System::Pinout::Name data2 = System::Pinout::Name::kDisable;
			System::Pinout::Name data3 = System::Pinout::Name::kDisable;
			System::Pinout::Name data4 = System::Pinout::Name::kDisable;
			System::Pinout::Name data5 = System::Pinout::Name::kDisable;
			System::Pinout::Name data6 = System::Pinout::Name::kDisable;
			System::Pinout::Name data7 = System::Pinout::Name::kDisable;
			System::Pinout::Name data8 = System::Pinout::Name::kDisable;
			System::Pinout::Name data9 = System::Pinout::Name::kDisable;
			System::Pinout::Name data10 = System::Pinout::Name::kDisable;
			System::Pinout::Name data11 = System::Pinout::Name::kDisable;
			System::Pinout::Name data12 = System::Pinout::Name::kDisable;
			System::Pinout::Name data13 = System::Pinout::Name::kDisable;
			System::Pinout::Name data14 = System::Pinout::Name::kDisable;
			System::Pinout::Name data15 = System::Pinout::Name::kDisable;
			System::Pinout::Name data16 = System::Pinout::Name::kDisable;
			System::Pinout::Name data17 = System::Pinout::Name::kDisable;
			System::Pinout::Name data18 = System::Pinout::Name::kDisable;
			System::Pinout::Name data19 = System::Pinout::Name::kDisable;
			System::Pinout::Name data20 = System::Pinout::Name::kDisable;
			System::Pinout::Name data21 = System::Pinout::Name::kDisable;
			System::Pinout::Name data22 = System::Pinout::Name::kDisable;
			System::Pinout::Name data23 = System::Pinout::Name::kDisable;
		};
		PinList pin_list;
		uint16_t width = 320u;  /*!< Pixels of the input frame. */
		uint16_t height = 240u; /*!< Lines of the input frame.  */
		bool hsync_active_high = true;
		bool data_latch_on_rising_edge = true;
		bool vsync_active_low = false;
		bool is_10_bit_data = false;
		uint8_t bytes_per_pixel = 2u;
		/*!< Bytes per pixel, valid values are:
		 - 2: Used for RGB565, YUV422, and so on.
		 - 3: Used for packed RGB888, packed YUV444, and so on.
		 - 4: Used for XRGB8888, XYUV444, and so on.
		 */
		uint16_t line_pitch_bytes = 320u * 2u;			/*!< Frame buffer line pitch, must be 8-byte aligned. */
		WorkMode work_mode = WorkMode::kGatedClockMode; /*!< CSI work mode. */
		DataBus data_bus = DataBus::k8Bit;				/*!< Data bus width. */
		bool use_ext_vsync = true;						/*!< In CCIR656 progressive mode, set true to use external VSYNC signal, set false to use internal VSYNC signal decoded from SOF. */
		Csi_Listener listener = nullptr;
		uint8_t interrupt_priority = 15; //Interrupt priority, range [0-15], smaller value means higher priority
	};
	Csi(const Config &config);
	void Start();
	void Stop();
	/*
	 *
	 */
	void ConfigTransferBuffer(uint8_t index, uint32_t addr);
	uint32_t GetTransferBuffer(uint8_t index);
	/*
	 * reval bit 0 asserted means first buffer ready bit 1 asserted means second buffer ready
	 */
	uint8_t IsTransferComplete()
	{
		return ((csi_base->CSISR >> 19) & 3u);
	}
	Csi_Listener GetListener()
	{
		return listener;
	}
	void SetListener(Csi_Listener listener, const uint8_t interrupt_priority);
	inline void SetInterrupt(bool enable){
		if(enable){
			csi_base->CSICR1 |= (0 | CSI_CSICR1_FB2_DMA_DONE_INTEN_MASK | CSI_CSICR1_FB1_DMA_DONE_INTEN_MASK);
		}else{
			csi_base->CSICR1 &= ~(0 | CSI_CSICR1_FB2_DMA_DONE_INTEN_MASK | CSI_CSICR1_FB1_DMA_DONE_INTEN_MASK);
		}
	}

  private:
	void Reset();
	/*
	 * param: bit 0 refer to rx fifo, bit 1 refer to static fifo
	 */
	void ClearFIFO(uint8_t fifo);
	/*
	 * param: bit 0 refer to rx fifo, bit 1 refer to static fifo
	 */
	void ReflashFIFODma(uint8_t fifo);
	/*
	 * param: fifo_enable bit 0 control wherether fifo is enable, bit 1 refer to rx fifo, bit 2 refer to static fifo
	 */
	void EnableFIFODmaRequest(uint8_t fifo_enable);

	Csi_Listener listener;
	CSI_Type *csi_base;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_CSI_H_ */
