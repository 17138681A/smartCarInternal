/*
 * MT9V034.h
 *
 *  Created on: Sep 15, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_MT9V034_H_
#define INC_DEVICE_DRIVER_MT9V034_H_

#include "driver/csi.h"
#include "driver/i2c_master.h"
#include "../config/config.h"

#ifdef MT9V034_USED

namespace DeviceDriver
{

/*!
 * @addtogroup cam Camera
 * @ingroup device_driver
 * @{
 */
/**
 * @brief MT9V034 grayscale camera driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class MT9V034
{
  public:
	struct Config
	{
		enum struct HDR
		{
			kDisable,
			k80dB,
			k100dB
		};
		uint8_t id = 0;							/*!<id of the camera */
		uint16_t width = 752;					/*!< Width of the camera image, need to be multiply of 8 or the program will do rounding by itself */
		uint16_t height = 480;					/*!< Height of the camera image */
		HDR hdr_mode = HDR::kDisable;			/*!< Preferred camera HDR mode */
		bool enable_AEC = false;				/*!< Option for auto explosure control */
		bool enable_AGC = false;				/*!< Option for auto gain control */
		uint8_t fps = 60;						/*!< This will config camera as close to the actually fps as possible, it will be affected by AEC */
		Driver::I2CMaster *i2c_master = nullptr;/*!< Optional i2c module for camera setting */
	};
	MT9V034(const Config &config);

	/**
	 * @brief Start the camera to capture frames
	 * 
	 */
	void Start();

	/**
	 * @brief Stop the camera from capturing frames
	 * 
	 */
	void Stop();

	/**
	 * @brief Get the width of the camera
	 * 
	 * @return uint16_t width of the camera capturing now
	 */
	inline uint16_t GetWidth() const
	{
		return width;
	}

	/**
	 * @brief Get the height of the camera
	 * 
	 * @return uint16_t height of the camera capturing now 
	 */
	inline uint16_t GetHeight() const
	{
		return height;
	}

	/**
	 * @brief Check new frame had been captured or not
	 * 
	 * @return true:  frame had been captured
	 * @return false: still waiting new frame coming
	 */
	inline bool IsNewFrame() const
	{
		return transferred;
	}

	/**
	 * @brief Get the latest captured frame
	 * 
	 * @return const uint8_t* pointer to the image data, size would be GetWidth() * GetHeight() \n
	 * 	 	   				  ordering of data would be (x=0,y=0), (x=1,y=0), (x=2,y=0)... (x=0,y=1), (x=1,y=1)...
	 */
	inline const uint8_t *LockBuffer()
	{
		transferred = false;
		csi.SetInterrupt(false);
		volatile uint32_t temp = waiting_buffer_addr;
		waiting_buffer_addr = ready_buffer_addr;
		ready_buffer_addr = temp;
		csi.SetInterrupt(true);
		return (uint8_t *)ready_buffer_addr;

	}

  private:
	void RegSet(uint8_t reg_addr, uint16_t value, bool check = true);
	uint16_t RegGet(uint8_t reg_addr);

	static const Driver::I2CMaster::Config GetI2CConfig(System::Pinout::Name sck, System::Pinout::Name sda);
	static const Driver::Csi::Config GetCsiConfig(const MT9V034::Config &camera_config);

	Driver::I2CMaster *i2c_master;
	Driver::Csi csi;
	uint16_t width, height;
	bool is_started = false;

	uint32_t ready_buffer_addr;
	uint32_t waiting_buffer_addr;
	volatile bool transferred;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* INC_DEVICE_DRIVER_MT9V034_H_ */
