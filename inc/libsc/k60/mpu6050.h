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

#include "libbase/k60/soft_i2c_master.h"
#include "libbase/misc_types.h"

namespace libsc
{
namespace k60
{

class Mpu6050
{
public:
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
	};

	explicit Mpu6050(const Config &config);

	void Update();

	const std::array<float, 3>& GetAccel()
	{
		return m_accel;
	}

	const std::array<float, 3>& GetOmega()
	{
		return m_omega;
	}

	float GetCelsius()
	{
		return m_temp;
	}

private:
	float GetGyroScaleFactor();
	float GetAccelScaleFactor();

	libbase::k60::SoftI2cMaster m_i2c;
	std::array<float, 3> m_accel;
	std::array<float, 3> m_omega;
	float m_temp;

	Config::Range m_gyro_range;
	Config::Range m_accel_range;
};

} /* namespace k60 */
} /* namespace libsc */
