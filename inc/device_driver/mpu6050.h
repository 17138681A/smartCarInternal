/*
 * mpu6050.h
 *
 *  Created on: Sep 21, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DEVICE_DRIVER_MPU6050_H_
#define INC_DEVICE_DRIVER_MPU6050_H_

#include "driver/i2c_master.h"
#include "../config/config.h"
#include <array>

#ifdef MPU6050_USED

namespace DeviceDriver
{

/*!
 * @addtogroup imu IMU
 * @ingroup device_driver
 * @{
 */
/**
 * @brief mpu6050 driver
 * 
 * ## Example
 * ~~~~~~~~~~~~~~~{.cpp}
 * ~~~~~~~~~~~~~~~
 */
class MPU6050
{
  public:
	struct Config
	{
		enum struct Range
		{
			kSmall,
			kMid,
			kLarge,
			kExtreme,
		};

		uint8_t id;
		// kSmall -> kExtreme = ±250°/s, ±500°/s, ±1000°/s, ±2000°/s
		Range gyro_range;
		// kSmall -> kExtreme = ±2g, ±4g, ±8g, ±16g
		Range accel_range;

		/// Calibrate the gyroscope while initializing
		bool cal_drift = false;

		Driver::I2CMaster *i2c_master = nullptr;
	};
	MPU6050(Config &config);
	bool Update(bool clamp_ = true);
	bool UpdateF(bool clamp_ = true);

	const int32_t *GetAccel() const
	{
		return accel;
	}
	const float *GetAccelF() const
	{
		return accel_f;
	}

	const int32_t *GetOmega() const
	{
		return omega;
	}
	const float *GetOmegaF() const
	{
		return omega_f;
	}

	float GetCelsius() const
	{
		return temp;
	}

	bool IsCalibrated() const
	{
		return is_calibrated;
	}

	const int32_t *GetOffset() const
	{
		return omega_offset;
	}
	const float *GetOffsetF() const
	{
		return omega_offset_f;
	}

	bool Verify();

	Driver::I2CMaster *GetI2cMaster()
	{
		return i2c_master;
	}

	uint16_t GetGyroScaleFactor(void) const
	{
		return gyro_scale_factor;
	}

	uint16_t GetAccelScaleFactor(void) const
	{
		return accel_scale_factor;
	}

  private:
	void Calibrate();
	void CalibrateF();

	uint16_t GetGyroScaleFactor();
	uint16_t GetAccelScaleFactor();

	Driver::I2CMaster *i2c_master;
	int32_t accel[3];
	int32_t omega[3];
	float accel_f[3];
	float omega_f[3];
	int32_t omega_offset[3];
	float omega_offset_f[3];
	float temp;
	bool is_calibrated;
	Config::Range gyro_range;
	Config::Range accel_range;
	uint16_t gyro_scale_factor;
	uint16_t accel_scale_factor;
};
/** @}*/

} // namespace DeviceDriver

#endif

#endif /* INC_DEVICE_DRIVER_MPU6050_H_ */
