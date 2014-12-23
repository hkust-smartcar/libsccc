/*
 * mpu6050.cpp
 *
 * Author: Harrison Ng
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cstdint>
#include <vector>

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

Mpu6050::Mpu6050():
	m_i2c(GetI2CConfig())
{
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1,0x00);

	//Register 25 – Sample Rate Divider: Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
	//Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or 7)
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV,0x00);

	//Register 26 - CONFIG: EXT_SYNC_SET[2:0]<<3 | DLPF_CFG[2:0];
	//EXT_SYNC_SET=0, Input disabled;
	//DLPF_CFG=0, Accel = 260Hz, Gyroscope = 256Hz;
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_CONFIG,0x00);

	//Register 27 - GYRO_CONFIG: FS_SEL[1:0] << 3;
	//FS_SEL=0, ± 250 °/s; FS_SEL=1, ± 500 °/s; FS_SEL=2, ± 1000 °/s; FS_SEL=3, ± 2000 °/s;
	m_gyro_config = 2 << 3;
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_CONFIG,m_gyro_config);

	//Register 28 - ACCEL_CONFIG: AFS_SEL[1:0] << 3;
	//AFS_SEL=0, ±2g; AFS_SEL=1, ±4g; AFS_SEL=2, ±8g; AFS_SEL=3, ±16g;
	m_accel_config = 2 << 3;
	m_i2c.SendByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_CONFIG,m_accel_config);
}

Mpu6050::~Mpu6050() {
}

float Mpu6050::GetGyroScaleFactor(){
	if(m_gyro_config == 0x0){
		return 131.0f;
	}
	if(m_gyro_config == (1 << 3)){
		return 65.5f;
	}
	if(m_gyro_config == (2 << 3)){
		return 32.8f;
	}
	if(m_gyro_config == (3 << 3)){
		return 16.4f;
	}
}

float Mpu6050::GetAccelScaleFactor(){
	if(m_accel_config == 0x0){
		return 16384.0f;
	}
	if(m_accel_config == (1 << 3)){
		return 8192.0f;
	}
	if(m_accel_config == (2 << 3)){
		return 4096.0f;
	}
	if(m_accel_config == (3 << 3)){
		return 2048.0f;
	}
}


void Mpu6050::Update(){
	vector<Byte> data = m_i2c.GetBytes(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 14);
	int j;
	for(int i = 0; i < data.size(); i+=2){
		if(i>=0 && i<=5){
			j = i/2;
			m_raw_acc[j] = data[i] << 8 | data[i+1];
			m_acc[j] = (float) m_raw_acc[j] * 1 / GetAccelScaleFactor();
		}
		if(i==6){
			m_raw_temp = data[i] << 8 | data[i+1];
			m_temp = (float) m_raw_temp / 340 + 36.53;
		}
		if(i>=8 && i<=13){
			j = (i - 8)/2;
			m_raw_gyro[j] = data[i] << 8 | data[i+1];
			m_omega[j] = (float) m_raw_gyro[j] * 1 / GetGyroScaleFactor();
		}
	}
}

float* Mpu6050::GetAcc(){
	return m_acc;
}

float* Mpu6050::GetOmega(){
	return m_omega;
}

float Mpu6050::GetCelsius(){
	return m_temp;
}

#else
Mpu6050::Mpu6050()
		: m_i2c(nullptr), m_raw_temp(0), m_temp(0), m_gyro_config(0),
		  m_accel_config(0)
{}
Mpu6050::~Mpu6050() {}
void Mpu6050::Update() {}
float* Mpu6050::GetAcc() { return nullptr; }
float* Mpu6050::GetOmega() { return nullptr; }
float Mpu6050::GetCelsius() { return 0.0f; }

#endif /* LIBSC_USE_MPU6050 */

} /* namespace k60 */
} /* namespace libsc */
