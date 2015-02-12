/*
 * soft_sccb_master.h
 * OmniVision Serial Camera Control Bus master
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/gpio.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/soft_i2c_master.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace k60
{

class SoftSccbMaster
{
public:
	struct Config
	{
		// 3-Wire SCCB is currently NOT supported
		//Pin::Name sccb_e_pin = Pin::Name::kDisable;
		Pin::Name sio_c_pin;
		Pin::Name sio_d_pin;
	};

	explicit SoftSccbMaster(const Config &config);
	explicit SoftSccbMaster(nullptr_t);
	SoftSccbMaster(const SoftSccbMaster&) = delete;
	SoftSccbMaster(SoftSccbMaster &&rhs);
	~SoftSccbMaster();

	SoftSccbMaster& operator=(const SoftSccbMaster&) = delete;
	SoftSccbMaster& operator=(SoftSccbMaster &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	bool GetByte(const Byte slave_addr, const Byte reg_addr, Byte *out_byte);
	bool SendByte(const Byte slave_addr, const Byte reg_addr, const Byte byte);

private:
	void Uninit();

	//Gpo m_sccb_e;
	SoftI2cMaster m_i2c;

	bool m_is_init;
};

}
}


