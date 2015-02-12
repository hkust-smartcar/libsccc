/*
 * soft_sccb.cpp
 * OmniVision Serial Camera Control Bus
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/pin.h"
#include "libbase/k60/soft_i2c_master.h"
#include "libbase/k60/soft_sccb_master.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace k60
{

namespace
{

SoftI2cMaster::Config GetI2cConfig(const SoftSccbMaster::Config &config)
{
	SoftI2cMaster::Config product;
	product.scl_pin = config.sio_c_pin;
	product.sda_pin = config.sio_d_pin;
	product.is_use_repeated_start = false;
	return product;
}

//Gpo::Config GetSccbEConfig(const SoftSccbMaster::Config &config)
//{
//	Gpo::Config product;
//	product.pin = config.sccb_e_pin;
//	product.is_high = true;
//	return product;
//}

}

SoftSccbMaster::SoftSccbMaster(const Config &config)
		: //m_sccb_e(nullptr),
		  m_i2c(GetI2cConfig(config)),
		  m_is_init(true)
{
//	if (config.sccb_e_pin != Pin::Name::kDisable)
//	{
//		m_sccb_e = Gpo(GetSccbEConfig(config));
//	}
}

SoftSccbMaster::SoftSccbMaster(SoftSccbMaster &&rhs)
		: SoftSccbMaster(nullptr)
{
	*this = std::move(rhs);
}

SoftSccbMaster::SoftSccbMaster(nullptr_t)
		: //m_sccb_e(nullptr),
		  m_i2c(nullptr),
		  m_is_init(false)
{}

SoftSccbMaster::~SoftSccbMaster()
{
	Uninit();
}

SoftSccbMaster& SoftSccbMaster::operator=(SoftSccbMaster &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			//m_sccb_e = std::move(rhs.m_sccb_e);
			m_i2c = std::move(rhs.m_i2c);

			m_is_init = true;
		}
	}
	return *this;
}

void SoftSccbMaster::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		//m_sccb_e = Gpo(nullptr);
		m_i2c = SoftI2cMaster(nullptr);
	}
}

bool SoftSccbMaster::GetByte(const Byte slave_addr, const Byte reg_addr,
		Byte *out_byte)
{
	return m_i2c.GetByte(slave_addr, reg_addr, out_byte);
}

bool SoftSccbMaster::SendByte(const Byte slave_addr, const Byte reg_addr,
		const Byte byte)
{
	return m_i2c.SendByte(slave_addr, reg_addr, byte);
}

}
}
