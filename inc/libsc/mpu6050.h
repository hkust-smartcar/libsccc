/*
 * mpu6050.h
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <array>

#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(i2c_master)
#include LIBBASE_H(soft_i2c_master)

#include "libsc/config.h"

namespace libsc
{

class Mpu6050
{
public:
#if LIBSC_USE_SOFT_MPU6050
	typedef LIBBASE_MODULE(SoftI2cMaster) I2cMaster;

#else
	typedef LIBBASE_MODULE(I2cMaster) I2cMaster;

#endif // LIBSC_USE_SOFT_MPU6050

	struct Config
	{
		enum struct Range
		{
			kSmall = 0,
			kMid,
			kLarge,
			kExtreme,
		};

		// kSmall -> kExtreme = ±250°/s, ±500°/s, ±1000°/s, ±2000°/s
		Range gyro_range;
		// kSmall -> kExtreme = ±2g, ±4g, ±8g, ±16g
		Range accel_range;

		/// Calibrate the gyroscope while initializing
		bool cal_drift = false;

	};

	explicit Mpu6050(const Config &config);

	bool Update(bool clamp_ = true);

	const std::array<float, 3>& GetAccel() const
	{
		return m_accel;
	}

	const std::array<float, 3>& GetOmega() const
	{
		return m_omega;
	}

	float GetCelsius() const
	{
		return m_temp;
	}

	bool IsCalibrated() const
	{
		return m_is_calibrated;
	}

	const std::array<float, 3>& GetOffset() const
	{
		return m_omega_offset;
	}

private:
	bool Verify();
	void Calibrate();

	float GetGyroScaleFactor();
	float GetAccelScaleFactor();

	I2cMaster m_i2c;
	std::array<float, 3> m_accel;
	std::array<float, 3> m_omega;
	std::array<float, 3> m_omega_offset;
	float m_temp;
	bool m_is_calibrated;

	Config::Range m_gyro_range;
	Config::Range m_accel_range;
};

}
