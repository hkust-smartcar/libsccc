/*
 * mpu6050.h
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

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
	float* GetAcc();
	float* GetOmega();
	float GetCelsius();

private:
	libbase::k60::SoftI2cMaster m_i2c;
	int16_t m_raw_acc[3];
	int16_t m_raw_gyro[3];
	int16_t m_raw_temp;
	float m_acc[3];
	float m_omega[3];
	float m_temp;
	Byte m_gyro_config;
	Byte m_accel_config;

	float GetGyroScaleFactor();
	float GetAccelScaleFactor();
};

} /* namespace k60 */
} /* namespace libsc */
