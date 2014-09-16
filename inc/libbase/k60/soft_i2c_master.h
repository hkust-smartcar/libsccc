/*
 * soft_i2c_master.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <vector>

#include "libbase/k60/gpio.h"
#include "libbase/k60/i2c_master_interface.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class SoftI2cMaster : public I2cMasterInterface
{
public:
	struct Config : public I2cMasterInterface::Config
	{
		// Try pushing your slave device to its limit! The default is for the
		// standard 100KHz bus
		uint32_t delay_us = 10;
	};

	explicit SoftI2cMaster(const Config &config);
	explicit SoftI2cMaster(nullptr_t);
	SoftI2cMaster(const SoftI2cMaster&) = delete;
	SoftI2cMaster(SoftI2cMaster &&rhs);
	~SoftI2cMaster();

	SoftI2cMaster& operator=(const SoftI2cMaster&) = delete;
	SoftI2cMaster& operator=(SoftI2cMaster &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	bool GetByte(const Byte slave_addr, const Byte reg_addr, Byte *out_byte) override;
	std::vector<Byte> GetBytes(const Byte slave_addr, const Byte reg_addr,
			const uint8_t size) override;
	bool SendByte(const Byte slave_addr, const Byte reg_addr, const Byte byte) override;
	bool SendBytes(const Byte slave_addr, const Byte reg_addr,
			const std::vector<Byte> &bytes) override;

private:
	void Uninit();

	void Start();
	void Stop();
	bool SendByte_(const Byte byte);
	Byte ReadByte_(const bool is_ack);

	inline void Delay();

	uint32_t m_delay_us;

	Gpio m_scl;
	Gpio m_sda;

	bool m_is_init;
};

}
}