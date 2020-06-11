/*
 * elcdif.h
 *
 *  Created on: Sep 14, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_ELCDIF_H_
#define INC_DRIVER_ELCDIF_H_

#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/pinout.h"
#include <functional>

namespace Driver
{

/*!
 * @addtogroup elcdif Enhanced LCD Interface
 * @ingroup driver
 * The enhanced Liquid Crystal Display Interface (LCDIF) is a general purpose display controller
 * @{
 */
/**
 * @brief Enhanced LCD Interface
 */
class eLCDIF
{
  public:
	typedef std::function<void(eLCDIF *)> eLCDIF_Listener;
	struct Config
	{
		enum struct PixelFormat
		{
			kRAW8 = 0,	 /*!< RAW 8 bit, four data use 32 bits. */
			kRGB565 = 1,   /*!< RGB565, two pixel use 32 bits. */
			kRGB666 = 2,   /*!< RGB666 unpacked, one pixel uses 32 bits, high byte unused, upper 2 bits of other bytes unused. */
			kXRGB8888 = 3, /*!< XRGB8888 unpacked, one pixel uses 32 bits, high byte unused. */
			kRGB888 = 4,   /*!< RGB888 packed, one pixel uses 24 bits. */
		};
		enum struct DataBus
		{
			k8Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(1),  /*!< 8-bit data bus. */
			k16Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(0), /*!< 16-bit data bus, support RGB565. */
			k18Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(2), /*!< 18-bit data bus, support RGB666. */
			k24Bit = LCDIF_CTRL_LCD_DATABUS_WIDTH(3), /*!< 24-bit data bus, support RGB888. */
		};
		struct PinList
		{
			System::Pinout::Name pclk = System::Pinout::Name::kDisable;
			System::Pinout::Name vsync = System::Pinout::Name::kDisable;
			System::Pinout::Name hsync = System::Pinout::Name::kDisable;
			System::Pinout::Name enable = System::Pinout::Name::kDisable;
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
		uint16_t width = 480;  /*!< Display panel width, pixels per line. */
		uint16_t height = 272; /*!< Display panel height, how many lines per panel. */
		uint8_t frame_rate = 60;
		uint8_t hsync_pulse_width = 41;		/*!< HSYNC pulse width. */
		uint8_t horizontal_front_porch = 4; /*!< Horizontal front porch. */
		uint8_t horizontal_back_porch = 8;  /*!< Horizontal back porch. */
		uint8_t vsync_pulse_width = 10;		/*!< VSYNC pulse width. */
		uint8_t vertical_front_porch = 4;   /*!< Vrtical front porch. */
		uint8_t vertical_back_porch = 2;	/*!< Vertical back porch. */
		bool vsync_active_high = false;
		bool hsync_active_high = false;
		bool data_enable_active_high = false;
		bool drive_data_on_rising_clk_edge = false;
		uint32_t buffer_addr = 0;						 /*!< Frame buffer address. */
		PixelFormat pixel_format = PixelFormat::kRGB888; /*!< Pixel format. */
		DataBus data_bus = DataBus::k24Bit;				 /*!< LCD data bus. */
		eLCDIF_Listener listener = nullptr;
		uint8_t interrupt_priority = 15; //Interrupt priority, range [0-15], smaller value means higher priority
	};
	eLCDIF(const Config &config);
	inline void Start()
	{
		elcdif_base->CTRL_SET = LCDIF_CTRL_RUN_MASK | LCDIF_CTRL_DOTCLK_MODE_MASK;
	}
	inline void Stop()
	{
		elcdif_base->CTRL_CLR = LCDIF_CTRL_DOTCLK_MODE_MASK;
	}
	inline eLCDIF_Listener GetListener()
	{
		return listener;
	}
	void SetListener(eLCDIF_Listener listener, uint8_t interrupt_priority);
	inline void SetNextBufferAddr(uint32_t addr)
	{
		elcdif_base->NEXT_BUF = addr;
	}
	inline void SetCurrBufferAddr(uint32_t addr)
	{
		elcdif_base->CUR_BUF = addr;
	}

  private:
	void Reset();
	eLCDIF_Listener listener;
	LCDIF_Type *elcdif_base;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_ELCDIF_H_ */
