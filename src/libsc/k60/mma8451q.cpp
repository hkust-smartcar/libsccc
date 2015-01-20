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

SoftI2cMaster::Config GetI2cMasterConfig()
{
	SoftI2cMaster::Config config;
//	config.scl_pin = Pin::Name::kPta4;
//	config.sda_pin = Pin::Name::kPta6;
	config.scl_pin = Pin::Name::kPtb0;
	config.sda_pin = Pin::Name::kPtb1;
	return config;
}

bool Mma8451q::IsConnected()
{
	Byte devId = 0;
	devId = ReadRegByte(MMA8451Q_RA_REG_WHO_AM_I);
	return (devId == 0x1A);
}

float Mma8451q::GetAccelGeneral(const Byte MsbRegAddr)
{
	Byte *bytes = new Byte[2] { 0 };
	uint16_t hbytes = 0;

	bytes = ReadRegBytes(MsbRegAddr, (Byte)m_Len);

	if ((Byte)m_Len - 1)
	{
		hbytes = bytes[0] << 8 | bytes[1];
		hbytes = (bytes[0] >> 7)? ((~hbytes + (0x01 << 2)) + 0x01) : (hbytes);
		hbytes >>= 2;
	}
	else
	{
		hbytes = (bytes[0] >> 7)? (~bytes[0] + 0x01) : (bytes[0]);
		hbytes <<= 8;
		hbytes >>= 2;
	}

	return ((float)hbytes / (float)m_Sens * ((bytes[0] >> 7)? -1 : 1));
}

float Mma8451q::GetAccelX()
{
	if (ReadRegByte(MMA8451Q_RA_REG_STATUS) & MMA8451Q_S_XDR)
		m_lastAccelX = GetAccelGeneral(MMA8451Q_RA_REG_OUT_X_MSB);
	return m_lastAccelX;
}

float Mma8451q::GetAccelY()
{
	if (ReadRegByte(MMA8451Q_RA_REG_STATUS) & MMA8451Q_S_YDR)
		m_lastAccelY = GetAccelGeneral(MMA8451Q_RA_REG_OUT_Y_MSB);
	return m_lastAccelY;
}

float Mma8451q::GetAccelZ()
{
	if (ReadRegByte(MMA8451Q_RA_REG_STATUS) & MMA8451Q_S_ZDR)
		m_lastAccelZ = GetAccelGeneral(MMA8451Q_RA_REG_OUT_Z_MSB);
	return m_lastAccelZ;
}

array<float, 3> Mma8451q::GetAccel()
{
	array<float, 3> ret = { 0 };

	ret[0] = GetAccelX();
	ret[1] = GetAccelY();
	ret[2] = GetAccelZ();

	return ret;
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
	m_I2cMaster(GetI2cMasterConfig()),
	m_Sens(config.sens),
	m_Len(config.len)
{
	if (config.id != 0)
		assert(false);
//	WriteRegByte(MMA8451Q_RA_CTRL_REG4, MMA8451Q_CR4_INT_EN_DRDY |
//											 MMA8451Q_CR4_INT_EN_FF_MT |
//											 MMA8451Q_CR4_INT_EN_PULSE |
//											 MMA8451Q_CR4_INT_EN_TRANS |
//											 MMA8451Q_CR4_INT_EN_FIFO);
//	WriteRegByte(MMA8451Q_RA_CTRL_REG5, MMA8451Q_CR5_INT_CFG_DRDY |
//											 MMA8451Q_CR5_INT_CFG_PULSE |
//											 MMA8451Q_CR4_INT_EN_TRANS);

	WriteRegByte(MMA8451Q_RA_XYZ_DATA_CFG, (Byte)config.sens - (Byte)0x0A);

	WriteRegByte(MMA8451Q_RA_HP_FILTER_CUTOFF, MMA8451Q_HFC_PULSE_LPF_EN);

	WriteRegByte(MMA8451Q_RA_CTRL_REG2, MMA8451Q_CR2_MODS0);

	WriteRegByte(MMA8451Q_RA_CTRL_REG1, (Byte)config.output_data_rate |
										MMA8451Q_CR1_F_ACTIVE |
										MMA8451Q_CR1_LNOISE);
}

Mma8451q::Mma8451q()
:
	m_I2cMaster(nullptr),
	m_Sens(Config::Sensitivity::Low),
	m_Len(Config::DataLength::k8)
{}

}

}
