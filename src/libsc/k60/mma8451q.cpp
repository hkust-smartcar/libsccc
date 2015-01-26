/*
 * mma8451q.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: Peter
 */

#include "libsc/k60/mma8451q.h"
#include "libsc/device_h/mma8451q.h"

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

SoftI2cMaster::Config GetI2cMasterConfig(Mma8451q::Config config)
{
	SoftI2cMaster::Config Masterconfig;
//	config.scl_pin = Pin::Name::kPta4;
//	config.sda_pin = Pin::Name::kPta6;
	Masterconfig.scl_pin = config.scl_pin;
	Masterconfig.sda_pin = config.sda_pin;
	return Masterconfig;
}

bool Mma8451q::IsConnected()
{
	uint8_t devId = 0;
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

float Mma8451q::GetAccelX()
{
	return m_lastAccel[0];
}
float Mma8451q::GetAccelY()
{
	return m_lastAccel[1];
}
float Mma8451q::GetAccelZ()
{
	return m_lastAccel[2];
}
array<float, 3> Mma8451q::GetAccel()
{
	return m_lastAccel;
}

float Mma8451q::GetAngleX()
{
	return m_lastAngle[0];
}
float Mma8451q::GetAngleY()
{
	return m_lastAngle[1];
}
float Mma8451q::GetAngleZ()
{
	return m_lastAngle[2];
}
array<float, 3> Mma8451q::GetAngle()
{
	return m_lastAngle;
}


// TODO: test it
// TODO: further simplify the following calculation
void Mma8451q::GetAllAccel()
{
	int16_t *bytes = new int8_t[6] { 0 };

	bytes = (int16_t *)ReadRegBytes(MMA8451Q_RA_REG_OUT_ALL, 0x06);
	for (uint8_t i = 0; i < 3; i++)
		m_lastAccel[i] = (float)(abs(bytes[i]) >> 2) / m_ScaleFactor * ((bytes[i] < 0)? -1 : 1);
}

void Mma8451q::GetAllAngle()
{
	for (uint8_t i = 0, j = 0, k = 0; i < 3; i++, j = (i + 1) % 3, k = (i + 2) % 3)
	{
		m_lastAngle[i] = ArcTan(m_lastAccel[i] / Sqrt2(m_lastAccel[j] * m_lastAccel[j] + m_lastAccel[k] * m_lastAccel[k]));
		if (!isfinite(m_lastAngle[i]))
			m_lastAngle[i] = halfPI;
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

uint8_t *Mma8451q::ReadRegBytes(const uint8_t RegAddr, const uint8_t Length)
{
	return m_I2cMaster.GetBytes(MMA8451Q_DEFAULT_ADDRESS, RegAddr, Length).data();
}

bool Mma8451q::WriteRegBytes(const uint8_t RegAddr, const uint8_t *data)
{
	return m_I2cMaster.SendBytes(MMA8451Q_DEFAULT_ADDRESS, RegAddr, vector<uint8_t>(data, data + sizeof(data) / sizeof(data[0])));
}

uint8_t Mma8451q::ReadRegByte(const uint8_t RegAddr)
{
	uint8_t data = 0;
	m_I2cMaster.GetByte(MMA8451Q_DEFAULT_ADDRESS, RegAddr, &data);
	return data;
}

bool Mma8451q::WriteRegByte(const uint8_t RegAddr, const uint8_t data)
{
	return m_I2cMaster.SendByte(MMA8451Q_DEFAULT_ADDRESS, RegAddr, data);
}

Mma8451q::Mma8451q(Mma8451q::Config config)
:
	m_I2cMaster(GetI2cMasterConfig(config)),
	m_Sens(config.sens),
	m_ScaleFactor((float)(1 << ((uint8_t)m_Sens + 0x0A)))
{
	if (config.id != 0)
		assert(false);

	WriteRegByte(MMA8451Q_RA_XYZ_DATA_CFG, 2 - (uint8_t)config.sens);

	WriteRegByte(MMA8451Q_RA_CTRL_REG2, (uint8_t)config.power_mode);

	WriteRegByte(MMA8451Q_RA_CTRL_REG1, (uint8_t)config.output_data_rate << 3 |
										MMA8451Q_CR1_LNOISE |
										MMA8451Q_CR1_F_ACTIVE);
}

Mma8451q::Mma8451q()
:
	m_I2cMaster(nullptr),
	m_Sens(Config::Sensitivity::Low),
	m_lastAccel({ 0.0f })
{}

}

}
