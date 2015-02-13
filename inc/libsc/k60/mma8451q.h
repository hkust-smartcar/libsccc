/*
 * mma8451q.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <cstdint>

#include "libbase/k60/soft_i2c_master.h"
#include "libbase/misc_types.h"

namespace libsc
{
namespace k60
{

class Mma8451q
{
public:
	struct Config
	{
		enum struct Sensitivity
		{
			High = 0,
			Mid,
			Low
		};

		enum struct ODR
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
			Normal = 0,
			LowNoiseLowPower,
			HighResolution,
			LowPower
		};

		uint8_t id;

		/** Maximun acceleration:
		 *  Low - 2g, Mid - 4g, High - 8g */
		Sensitivity sens = Sensitivity::Mid;

		PowerMode power_mode = PowerMode::LowNoiseLowPower;
		ODR output_data_rate = ODR::k200Hz;
	};

	explicit Mma8451q();
	explicit Mma8451q(const Mma8451q::Config &config);
	~Mma8451q();

	bool IsConnected();

	// 13ms
	bool Update();

	std::array<float, 3> GetAccel();
	std::array<float, 3> GetAngle();

private:
	// 2ms
	void GetAllAccel();
	// 0ms
	void GetAllAngle();

	float ArcTan(float x);
	float ArcSin(float x);
	float Sqrt2(const float x);

	Byte ReadRegByte(const Byte RegAddr);
	bool WriteRegByte(const Byte RegAddr, const Byte data);
	Byte* ReadRegBytes(const Byte RegAddr, const Byte Length);
	bool WriteRegBytes(const Byte RegAddr, const Byte *data);

	libbase::k60::SoftI2cMaster m_I2cMaster;
	Config::Sensitivity m_Sens;
	float m_ScaleFactor;

	std::array<float, 3> m_lastAccel;
	std::array<float, 3> m_lastAngle;

};

}

}
