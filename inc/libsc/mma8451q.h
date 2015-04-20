/*
 * mma8451q.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <array>
#include <vector>

#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(i2c_master)
#include LIBBASE_H(soft_i2c_master)

#include "libsc/config.h"

namespace libsc
{

class Mma8451q
{
public:
#if LIBSC_USE_SOFT_MMA8451Q
	typedef LIBBASE_MODULE(SoftI2cMaster) I2cMaster;

#else
	typedef LIBBASE_MODULE(I2cMaster) I2cMaster;

#endif // LIBSC_USE_SOFT_MMA8451Q

	struct Config
	{
		enum struct Sensitivity
		{
			kHigh = 0,
			kMid,
			kLow
		};

		enum struct OutputDataRate
		{
			k800Hz = 0,
			k400Hz,
			k200Hz,
			k100Hz,
			k50Hz,
			k12_5Hz,
			k6_25Hz,
			k1_56Hz
		};

		enum struct PowerMode
		{
			kNormal = 0,
			kLowNoiseLowPower,
			kHighResolution,
			kLowPower
		};

		uint8_t id;

		/**
		 * Set the sensitivity of the accelerometer as follows:
		 * Sensitivity - Maximun acceleration
		 * Low - 2g
		 * Mid - 4g
		 * High - 8g
		 */
		Sensitivity sensitivity = Sensitivity::kLow;

		PowerMode power_mode = PowerMode::kNormal;
		OutputDataRate output_data_rate = OutputDataRate::k400Hz;

		I2cMaster* i2c_master_ptr = NULL;
	};

	explicit Mma8451q(const Config &config);

	bool Update();

	const std::array<float, 3>& GetAccel() const
	{
		return m_last_accel;
	}

	const std::array<float, 3>& GetAngle() const
	{
		return m_last_angle;
	}

	I2cMaster* GetI2cMaster(){
		return m_i2c_master;
	}

private:
	bool Verify();

	void GetAllAccel();
	void GetAllAngle();

	void SetActive(const bool flag);

	Byte ReadRegByte(const Byte reg);
	void WriteRegByte(const Byte reg, const Byte data);

	I2cMaster* m_i2c_master;
	Config::Sensitivity m_sensitivity;
	float m_scale_factor;

	std::array<float, 3> m_last_accel;
	std::array<float, 3> m_last_angle;

};

}
