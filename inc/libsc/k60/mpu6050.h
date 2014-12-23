/*
 * mpu6050.h
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <array>

#include "libbase/k60/soft_i2c_master.h"
#include "libbase/misc_types.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

class Mpu6050
{
public:
	Mpu6050();
	~Mpu6050();

	void Update();
	const std::array<float, 3>& GetAcc();
	const std::array<float, 3>& GetOmega();
	float GetCelsius();

private:
	libbase::k60::SoftI2cMaster m_i2c;
	int16_t m_raw_acc[3];
	int16_t m_raw_gyro[3];
	int16_t m_raw_temp;
	std::array<float, 3> m_acc;
	std::array<float, 3> m_omega;
	float m_temp;
	Byte m_gyro_config;
	Byte m_accel_config;

	float GetGyroScaleFactor();
	float GetAccelScaleFactor();
};

} /* namespace k60 */
} /* namespace libsc */
