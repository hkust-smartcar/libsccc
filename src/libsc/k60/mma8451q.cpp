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
	Byte devId = 0;
	devId = ReadRegByte(MMA8451Q_RA_REG_WHO_AM_I);
	return (devId == 0x1A);
}

//float Mma8451q::GetAccelGeneral(const Byte MsbRegAddr)
//{
//	Byte *bytes = new Byte[2] { 0 };
//	uint16_t hbytes = 0;
//
//	bytes = ReadRegBytes(MsbRegAddr, (Byte)m_Len);
//
//	if ((Byte)m_Len - 1)
//	{
//		hbytes = bytes[0] << 8 | bytes[1];
//		hbytes = (bytes[0] >> 7)? ((~hbytes + (0x01 << 2)) + 0x01) : (hbytes);
//		hbytes >>= 2;
//	}
//	else
//	{
//		hbytes = (bytes[0] >> 7)? (~bytes[0] + 0x01) : (bytes[0]);
//		hbytes <<= 8;
//		hbytes >>= 2;
//	}
//
//	return ((float)hbytes / ((float)(1 << ((Byte)m_Sens + 0x0A))) * ((bytes[0] >> 7)? -1 : 1));
//}

float Mma8451q::getAllAccel()
{
	Byte *bytes = new Byte[6] { 0 };
	uint16_t hbytes = 0;

	bytes = ReadRegBytes(MMA8451Q_RA_REG_OUT_X_MSB, 0x06);

	for (Byte i = 0; i < 6; i += 2)
	{
		if ((Byte)m_Len - 1)
		{
			hbytes = bytes[i] << 8 | bytes[i + 1];
			hbytes = (bytes[i] >> 7)? ((~hbytes + (0x01 << 2)) + 0x01) : (hbytes);
			hbytes >>= 2;
		}
		else
		{
			hbytes = (bytes[i] >> 7)? (~bytes[i] + 0x01) : (bytes[i]);
			hbytes <<= 8;
			hbytes >>= 2;
		}

		m_lastAccel[i / 2] = ((float)hbytes / ((float)(1 << ((Byte)m_Sens + 0x0A))) * ((bytes[i] >> 7)? -1 : 1));
	}
}

bool Mma8451q::update()
{
	if (ReadRegByte(MMA8451Q_RA_REG_STATUS) & MMA8451Q_S_ZYXDR)
	{
		getAllAccel();
		return true;
	}
	return false;
}

float Mma8451q::getAccelX()
{
	return m_lastAccel[0];
}

float Mma8451q::getAccelY()
{
	return m_lastAccel[1];
}

float Mma8451q::getAccelZ()
{
	return m_lastAccel[2];
}
array<float, 3> Mma8451q::getAccel()
{
	return m_lastAccel;
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
	m_Len(config.len)
{
	if (config.id != 0)
		assert(false);

	WriteRegByte(MMA8451Q_RA_XYZ_DATA_CFG, 2 - (Byte)config.sens);

	WriteRegByte(MMA8451Q_RA_CTRL_REG2, (Byte)config.power_mode);

	WriteRegByte(MMA8451Q_RA_CTRL_REG1, (Byte)config.output_data_rate << 3 |
										MMA8451Q_CR1_LNOISE |
										MMA8451Q_CR1_F_ACTIVE);
}

Mma8451q::Mma8451q()
:
	m_I2cMaster(nullptr),
	m_Sens(Config::Sensitivity::Low),
	m_Len(Config::DataLength::k8)
{}

}

}
