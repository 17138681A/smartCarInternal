/*
 * encoder.h
 *
 *  Created on: 31 Dec 2018
 *      Author: lch19
 */

#ifndef DEVICE_DRIVER_ENCODER_H_
#define DEVICE_DRIVER_ENCODER_H_

#include "driver/quad_decoder.h"
#include "../config/config.h"

#ifdef ENCODER_USED

namespace DeviceDriver
{

/*!
 * @addtogroup encoder Encoder
 * @ingroup device_driver
 * @{
 */
/**
 * @brief Encoder driver
 */
class Encoder
{
  public:
	struct Config{
		uint8_t id;
		enum struct Type{
			kABEncoder,kDirEncoder
		} type;
		bool reverse_counting = false;
	};

	Encoder(const Config& config);

	/**
	 * @brief Get the encoder count, also act as an update of encoder count
	 * 
	 * @return uint32_t encoder count value
	 */
	inline uint32_t GetCount(){
		return quad_decoder.GetPositionCount();
	}

	/**
	 * @brief Get the change of encoder count, use this value divided by time interval between each GetCount() called can get the velocity of the encoder rotate. \n
	 *        Need to call GetCount() before calling this function.
	 * 
	 * @return int16_t encoder count different
	 */
	inline int16_t GetChange(){
		return quad_decoder.GetHoldPositionDifferentValue();
	}

	private:
	static const Driver::QuadDecoder::Config GetDecoderConfig(const Config &config);

	Driver::QuadDecoder quad_decoder;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* DEVICE_DRIVER_ENCODER_H_ */
