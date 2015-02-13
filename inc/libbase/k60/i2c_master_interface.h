/*
 * i2c_master_interface.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class I2cMasterInterface
{
public:
	struct Config
	{
		Pin::Name scl_pin;
		Pin::Name sda_pin;
		uint16_t scl_low_timeout = UINT16_MAX;
	};

	virtual ~I2cMasterInterface()
	{}

	virtual operator bool() const = 0;

	virtual bool GetByte(const Byte slave_addr, const Byte reg_addr,
			Byte *out_byte)  = 0;
	virtual std::vector<Byte> GetBytes(const Byte slave_addr,
			const Byte reg_addr, const uint8_t size) = 0;
	virtual bool SendByte(const Byte slave_addr, const Byte reg_addr,
			const Byte byte) = 0;
	virtual bool SendBytes(const Byte slave_addr, const Byte reg_addr,
			const std::vector<Byte> &bytes) = 0;
};

}
}
