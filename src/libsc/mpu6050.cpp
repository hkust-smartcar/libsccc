/*
 * mpu6050.cpp
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cmath>
#include <cstdint>

#include <array>
#include <vector>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(i2c_master)
#include LIBBASE_H(soft_i2c_master)

#include "libsc/config.h"
#include "libsc/device_h/mpu6050.h"
#include "libsc/mpu6050.h"
#include "libsc/system.h"
#include "libutil/misc.h"

#define ABS(v) ((v < 0)? -v : v)

using namespace LIBBASE_NS;
using namespace std;

namespace libsc
{

#ifdef LIBSC_USE_MPU6050

namespace
{

Mpu6050::I2cMaster::Config GetI2cConfig()
{
	Mpu6050::I2cMaster::Config config;
	config.scl_pin = LIBSC_MPU6050_SCL;
	config.sda_pin = LIBSC_MPU6050_SDA;
	config.baud_rate_khz = 400;
	config.scl_low_timeout = 1000;
#if !LIBSC_USE_SOFT_MPU6050
	config.min_scl_start_hold_time_ns = 600;
	config.min_scl_stop_hold_time_ns = 600;
#endif
	return config;
}

}

Mpu6050::Mpu6050(const Config &config)
		:
		  m_accel{},
		  m_omega{},
		  m_omega_offset{},
		  m_accel_f{},
		  m_omega_f{},
		  m_omega_offset_f{},
		  m_temp(0.0f),
		  m_is_calibrated(false),
		  m_gyro_range(config.gyro_range),
		  m_accel_range(config.accel_range),
		  m_gyro_scale_factor(GetGyroScaleFactor()),
		  m_accel_scale_factor(GetAccelScaleFactor())
{
	if(!config.i2c_master_ptr){
		m_i2c = new I2cMaster(GetI2cConfig());
	}else{
		m_i2c = config.i2c_master_ptr;
	}

	assert(Verify());
	System::DelayUs(1);

	assert(m_i2c->SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0x03));
	System::DelayUs(1);

	//Register 25 – Sample Rate Divider: Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
	//Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or 7)
	assert(m_i2c->SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x01));
	System::DelayUs(1);

	//Register 26 - CONFIG: EXT_SYNC_SET[2:0]<<3 | DLPF_CFG[2:0];
	//EXT_SYNC_SET=0, Input disabled;
	//DLPF_CFG=0, Accel = 260Hz, Gyroscope = 256Hz;
	assert(m_i2c->SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_CONFIG, 0x00));
	System::DelayUs(1);

	//Register 27 - GYRO_CONFIG: FS_SEL[1:0] << 3;
	//FS_SEL=0, ± 250 °/s; FS_SEL=1, ± 500 °/s; FS_SEL=2, ± 1000 °/s; FS_SEL=3, ± 2000 °/s;
	uint8_t gyro_config = static_cast<int>(m_gyro_range) << 3;
	assert(m_i2c->SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_CONFIG,
			gyro_config));
	System::DelayUs(1);

	//Register 28 - ACCEL_CONFIG: AFS_SEL[1:0] << 3;
	//AFS_SEL=0, ±2g; AFS_SEL=1, ±4g; AFS_SEL=2, ±8g; AFS_SEL=3, ±16g;
	uint8_t accel_config = static_cast<int>(m_accel_range) << 3;
	assert(m_i2c->SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_CONFIG,
			accel_config));
	System::DelayUs(1);

	m_omega_offset[0] = 0;
	m_omega_offset[1] = 0;
	m_omega_offset[2] = 0;
	m_omega_offset_f[0] = 0;
	m_omega_offset_f[1] = 0;
	m_omega_offset_f[2] = 0;

	if (config.cal_drift)
	{
		Calibrate();
		CalibrateF();
	}
}

bool Mpu6050::Verify()
{
	Byte who_am_i;
	if (!m_i2c->GetByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_WHO_AM_I, &who_am_i))
	{
		return false;
	}
	else
	{
		return (who_am_i == 0x68);
	}
}

void Mpu6050::Calibrate()
{
	Timer::TimerInt t = 0, pt = 0;
	std::array<int32_t, 3> omega_sum{};

	int samples = 0, target_samples = 256;
	while (samples < target_samples)
	{
		t = System::Time();
		if (t - pt >= 5)
		{
			pt = t;
			Update(false);
			if (samples >= target_samples / 2)
			{
				std::array<int32_t, 3> omega_ = GetOmega();
				for (int i = 0; i < 3; i++)
					omega_sum[i] += omega_[i];
			}
			samples++;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		m_omega_offset[i] = omega_sum[i] / (target_samples/2);
	}
	m_is_calibrated = true;
}
void Mpu6050::CalibrateF()
{
	Timer::TimerInt t = 0, pt = 0;
	std::array<float, 3> omega_sum{};

	int samples = 0, target_samples = 256;
	while (samples < target_samples)
	{
		t = System::Time();
		if (t - pt >= 5)
		{
			pt = t;
			UpdateF(false);
			if (samples >= target_samples / 2)
			{
				std::array<float, 3> omega_ = GetOmegaF();
				for (int i = 0; i < 3; i++)
				{
					omega_sum[i] += omega_[i];
				}
			}
			samples++;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		m_omega_offset_f[i] = omega_sum[i] / (target_samples/2);
	}
	m_is_calibrated = true;
}

uint16_t Mpu6050::GetGyroScaleFactor()
{
	switch (m_gyro_range)
	{
	default:
		LOG_EL("Mpu6050 Gyro in illegal state");
	case Config::Range::kSmall:
		return 20960;

	case Config::Range::kMid:
		return 10480;

	case Config::Range::kLarge:
		return 5248;

	case Config::Range::kExtreme:
		return 2624;
	}
}

uint16_t Mpu6050::GetAccelScaleFactor()
{
	switch (m_accel_range)
	{
	default:
		LOG_EL("Mpu6050 Accel in illegal state");
	case Config::Range::kSmall:
		return 16384;

	case Config::Range::kMid:
		return 8192;

	case Config::Range::kLarge:
		return 4096;

	case Config::Range::kExtreme:
		return 2048;
	}
}

bool Mpu6050::Update(const bool clamp_)
{
	const vector<Byte> &data = m_i2c->GetBytes(MPU6050_DEFAULT_ADDRESS,
			MPU6050_RA_ACCEL_XOUT_H, 14);
	if (data.empty())
		return false;

	int16_t raw_accel[3]={0};
	int16_t raw_gyro[3]={0};
	for (size_t i = 0; i < data.size(); i += 2)
	{
		if (i <= 5)
		{
			const int j = i / 2;
			raw_accel[j] = (data[i] << 8) | data[i + 1];
			m_accel[j] = raw_accel[j];
		}
		else if (i == 6)
		{
			const int16_t raw_temp = (data[i] << 8) | data[i + 1];
			m_temp = (float)raw_temp / 340 + 36.53;
		}
		else
		{
			const int j = (i - 8) / 2;
			raw_gyro[j] = (data[i] << 8) | data[i + 1];
			m_omega[j] = raw_gyro[j] * 160;
			m_omega[j] -= m_omega_offset[j];
			if(clamp_)
				m_omega[j] = (ABS(m_omega[j]) < 3 * m_gyro_scale_factor)? 0 : m_omega[j];
		}
	}
	return true;
}
bool Mpu6050::UpdateF(const bool clamp_)
{
	const vector<Byte> &data = m_i2c->GetBytes(MPU6050_DEFAULT_ADDRESS,
			MPU6050_RA_ACCEL_XOUT_H, 14);
	if (data.empty())
	{
		return false;
	}

	int16_t raw_accel[3]={0};
	int16_t raw_gyro[3]={0};
	for (size_t i = 0; i < data.size(); i += 2)
	{
		if (i <= 5)
		{
			const int j = i / 2;
			raw_accel[j] = (data[i] << 8) | data[i + 1];
			m_accel_f[j] = (float)raw_accel[j] / m_accel_scale_factor;
		}
		else if (i == 6)
		{
			const int16_t raw_temp = (data[i] << 8) | data[i + 1];
			m_temp = (float)raw_temp / 340 + 36.53;
		}
		else
		{
			const int j = (i - 8) / 2;
			raw_gyro[j] = (data[i] << 8) | data[i + 1];
			m_omega_f[j] = (float)raw_gyro[j] * 160 / m_gyro_scale_factor;
			m_omega_f[j] -= m_omega_offset_f[j];
			if(clamp_) m_omega_f[j] = abs(m_omega_f[j]) < 3.0f ? 0.0f : m_omega_f[j];
		}
	}
	return true;
//	const vector<Byte> &data = m_i2c->GetBytes(MPU6050_DEFAULT_ADDRESS,
//			MPU6050_RA_GYRO_YOUT_H, 2);
//	if (data.empty())
//	{
//		return false;
//	}
//
//	int16_t raw_gyro[3]={0};
//
//
//	raw_gyro[1] = (data[0] << 8) | data[1];
//	m_omega_f[1] = (float)raw_gyro[1] / GetGyroScaleFactor();
//	m_omega_f[1] -= m_omega_offset_f[1];
//	if(clamp_) m_omega_f[1] = abs(m_omega_f[1]) < 1.0f ? 0.0f : m_omega_f[1];
//
//	return true;
}

#else
Mpu6050::Mpu6050(const Config&)
		: m_i2c(nullptr), m_temp(0), m_is_calibrated(false),
		  m_gyro_range(Config::Range::kSmall),
		  m_accel_range(Config::Range::kSmall)
{
	LOG_DL("Configured not to use Mpu6050");
}
bool Mpu6050::Update(const bool) { return false; }
bool Mpu6050::UpdateF(const bool) { return false; }

#endif /* LIBSC_USE_MPU6050 */

}
