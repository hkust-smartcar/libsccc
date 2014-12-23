/*
 * mpu6050.cpp
 *
 * Author: Harrison Ng
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cstdint>
#include <array>
#include <vector>

#include "libbase/log.h"
#include "libbase/k60/soft_i2c_master.h"

#include "libsc/config.h"
#include "libsc/device_h/mpu6050.h"
#include "libsc/k60/mpu6050.h"
#include "libutil/misc.h"

using namespace libbase::k60;
using namespace std;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_MPU6050

namespace
{

SoftI2cMaster::Config GetI2CConfig()
{
	SoftI2cMaster::Config config;
	config.scl_pin = LIBSC_MPU6050_SCL;
	config.sda_pin = LIBSC_MPU6050_SDA;
	return config;
}

}

Mpu6050::Mpu6050(const Config &config)
		: m_i2c(GetI2CConfig()),
		  m_gyro_range(config.gyro_range),
		  m_accel_range(config.accel_range)
{
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0x00);

	//Register 25 – Sample Rate Divider: Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
	//Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or 7)
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x00);

	//Register 26 - CONFIG: EXT_SYNC_SET[2:0]<<3 | DLPF_CFG[2:0];
	//EXT_SYNC_SET=0, Input disabled;
	//DLPF_CFG=0, Accel = 260Hz, Gyroscope = 256Hz;
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_CONFIG, 0x00);

	//Register 27 - GYRO_CONFIG: FS_SEL[1:0] << 3;
	//FS_SEL=0, ± 250 °/s; FS_SEL=1, ± 500 °/s; FS_SEL=2, ± 1000 °/s; FS_SEL=3, ± 2000 °/s;
	uint8_t gyro_config = static_cast<int>(m_gyro_range) << 3;
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_CONFIG, gyro_config);

	//Register 28 - ACCEL_CONFIG: AFS_SEL[1:0] << 3;
	//AFS_SEL=0, ±2g; AFS_SEL=1, ±4g; AFS_SEL=2, ±8g; AFS_SEL=3, ±16g;
	uint8_t accel_config = static_cast<int>(m_accel_range) << 3;
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_CONFIG,
			accel_config);
}

Mpu6050::~Mpu6050() {
}

float Mpu6050::GetGyroScaleFactor()
{
	switch (m_gyro_range)
	{
	default:
		LOG_EL("Mpu6050 Gyro in illegal state");
	case Config::Range::kSmall:
		return 131.0f;

	case Config::Range::kMid:
		return 65.5f;

	case Config::Range::kLarge:
		return 32.8f;

	case Config::Range::kExtreme:
		return 16.4f;
	}
}

float Mpu6050::GetAccelScaleFactor()
{
	switch (m_accel_range)
	{
	default:
		LOG_EL("Mpu6050 Accel in illegal state");
	case Config::Range::kSmall:
		return 16384.0f;

	case Config::Range::kMid:
		return 8192.0f;

	case Config::Range::kLarge:
		return 4096.0f;

	case Config::Range::kExtreme:
		return 2048.0f;
	}
}

void Mpu6050::Update()
{
	const vector<Byte> &data = m_i2c.GetBytes(MPU6050_DEFAULT_ADDRESS,
			MPU6050_RA_ACCEL_XOUT_H, 14);
	int16_t raw_acc[3];
	int16_t raw_gyro[3];
	for (size_t i = 0; i < data.size(); i += 2)
	{
		if (i >= 0 && i <= 5)
		{
			const int j = i / 2;
			raw_acc[j] = data[i] << 8 | data[i + 1];
			m_acc[j] = (float)raw_acc[j] / GetAccelScaleFactor();
		}
		else if (i == 6)
		{
			const int16_t raw_temp = data[i] << 8 | data[i + 1];
			m_temp = (float)raw_temp / 340 + 36.53;
		}
		else
		{
			const int j = (i - 8) / 2;
			raw_gyro[j] = data[i] << 8 | data[i + 1];
			m_omega[j] = (float)raw_gyro[j] / GetGyroScaleFactor();
		}
	}
}

const array<float, 3>& Mpu6050::GetAcc()
{
	return m_acc;
}

const array<float, 3>& Mpu6050::GetOmega()
{
	return m_omega;
}

float Mpu6050::GetCelsius()
{
	return m_temp;
}

#else
Mpu6050::Mpu6050(const Config&)
		: m_i2c(nullptr), m_raw_temp(0), m_temp(0),
		  m_gyro_range(Config::Range::kSmall),
		  m_accel_range(Config::Range::kSmall)
{}
Mpu6050::~Mpu6050() {}
void Mpu6050::Update() {}
const array<float, 3>& Mpu6050::GetAcc() { return m_acc; }
const array<float, 3>& Mpu6050::GetOmega() { return m_omega; }
float Mpu6050::GetCelsius() { return 0.0f; }

#endif /* LIBSC_USE_MPU6050 */

} /* namespace k60 */
} /* namespace libsc */
