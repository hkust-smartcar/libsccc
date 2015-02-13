/*
 * mma8451q.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cmath>
#include <cstdint>

#include <array>

#include "libsc/config.h"
#include "libsc/device_h/mma8451q.h"
#include "libsc/k60/mma8451q.h"
#include "libutil/misc.h"

// ArcSin : http://cpp.sh/7po
//			http://cpp.sh/5fqt

using namespace std;
using namespace libbase::k60;

#define PI				3.14159265f
#define halfPI			1.57079633f
#define SQRT_MAGIC_F	0x5F3759DF

namespace libsc
{

namespace k60
{

namespace
{

#if LIBSC_USE_MMA8451Q == 1
inline Pin::Name GetSclPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MMA8451Q0_SCL;
}

inline Pin::Name GetSdaPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MMA8451Q0_SDA;
}

#endif

SoftI2cMaster::Config GetI2cMasterConfig(const Mma8451q::Config &config)
{
	SoftI2cMaster::Config product;
	product.scl_pin = GetSclPin(config.id);
	product.sda_pin = GetSdaPin(config.id);
	return product;
}

}

bool Mma8451q::IsConnected()
{
	Byte devId = 0;
	devId = ReadRegByte(MMA8451Q_RA_REG_WHO_AM_I);
	return (devId == 0x1A);
}

bool Mma8451q::Update()
{
	if (ReadRegByte(MMA8451Q_RA_REG_STATUS) & MMA8451Q_S_ZYXDR)
	{
		GetAllAccel();
		GetAllAngle();
		return true;
	}
	return false;
}

array<float, 3> Mma8451q::GetAccel()
{
	return m_lastAccel;
}

array<float, 3> Mma8451q::GetAngle()
{
	return m_lastAngle;
}

void Mma8451q::GetAllAccel()
{
	Byte *bytes;

	bytes = ReadRegBytes(MMA8451Q_RA_REG_OUT_ALL, 0x06);

	for (Byte i = 0, j = 0; i < 3; i++, j += 2)
		m_lastAccel[i] = (float)((int16_t)(bytes[j] << 8 | bytes[j + 1]) / m_ScaleFactor);
}

void Mma8451q::GetAllAngle()
{
	for (uint8_t j = 0; j < 3; j++)
	{
		m_lastAngle[j] = ArcTan(m_lastAccel[j] / Sqrt2(m_lastAccel[(j + 1) % 3] * m_lastAccel[(j + 1) % 3] + m_lastAccel[(j + 2) % 3] * m_lastAccel[(j + 2) % 3]));
		if (!isfinite(m_lastAngle[j]))
			m_lastAngle[j] = halfPI;
	}
	m_lastAngle[2] -= halfPI * ((m_lastAccel[2] < 0)? -1 : 1);
}

/* Formula From Cecil Hastings */
float Mma8451q::ArcTan(float x)
{
	float y = (fabs(x) - 1) / (fabs(x) + 1);
	float z = y * y;
    return (.785398f + (.995354f + (-.288679f + .079331f * z) * z) * y) * ((x < 0)? -1 : 1);
}

float Mma8451q::ArcSin(float x)
{
	return (1.570796f - sqrt(1 - x) * (1.570723f - x * (0.212114f + x *( 0.074261f + x * (- 0.018729f)))));
}

float Mma8451q::Sqrt2(const float x)
{
	const float xhalf = 0.5f * x;
	union // get bits for floating value
	{
		float x;
		int i;
	} u;
	u.x = x;
	u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
	return x * u.x * (1.5f - xhalf * u.x * u.x);// Newton step, repeating increases accuracy
}

Byte *Mma8451q::ReadRegBytes(const Byte RegAddr, const Byte Length)
{
	return m_I2cMaster.GetBytes(MMA8451Q_DEFAULT_ADDRESS, RegAddr, Length).data();
}

bool Mma8451q::WriteRegBytes(const Byte RegAddr, const Byte *data)
{
	return m_I2cMaster.SendBytes(MMA8451Q_DEFAULT_ADDRESS, RegAddr, vector<Byte>(data, data + sizeof(data) / sizeof(data[0])));
}

Byte Mma8451q::ReadRegByte(const Byte RegAddr)
{
	Byte data = 0;
	m_I2cMaster.GetByte(MMA8451Q_DEFAULT_ADDRESS, RegAddr, &data);
	return data;
}

bool Mma8451q::WriteRegByte(const Byte RegAddr, const Byte data)
{
	return m_I2cMaster.SendByte(MMA8451Q_DEFAULT_ADDRESS, RegAddr, data);
}

Mma8451q::Mma8451q(Mma8451q::Config config)
:
	m_I2cMaster(GetI2cMasterConfig(config)),
	m_Sens(config.sens),
	m_ScaleFactor((float)(1 << ((Byte)m_Sens + 0x0C)))
{
	if (config.id != 0)
		assert(false);

	WriteRegByte(MMA8451Q_RA_CTRL_REG1, ReadRegByte(MMA8451Q_RA_CTRL_REG1) & ~MMA8451Q_CR1_F_ACTIVE);

	WriteRegByte(MMA8451Q_RA_XYZ_DATA_CFG, 2 - (Byte)config.sens);

//	WriteRegByte(MMA8451Q_RA_CTRL_REG2, (Byte)config.power_mode);
//
//	WriteRegByte(MMA8451Q_RA_CTRL_REG1, (Byte)config.output_data_rate << 3 | MMA8451Q_CR1_LNOISE);
	WriteRegByte(MMA8451Q_RA_CTRL_REG1, (Byte)config.output_data_rate << 3);

	WriteRegByte(MMA8451Q_RA_CTRL_REG1, ReadRegByte(MMA8451Q_RA_CTRL_REG1) | MMA8451Q_CR1_F_ACTIVE);
}

Mma8451q::Mma8451q()
:
	m_I2cMaster(nullptr),
	m_Sens(Config::Sensitivity::Low),
	m_ScaleFactor((float)(1 << ((Byte)m_Sens + 0x0C))),
	m_lastAccel({ 0.0f })
{}

}

}
