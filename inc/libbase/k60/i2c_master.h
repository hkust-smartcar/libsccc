/*
 * i2c_master.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <vector>

#include "libbase/k60/i2c_master_interface.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class I2cMaster : public I2cMasterInterface
{
public:
	struct Config : public I2cMasterInterface::Config
	{
		/**
		 * The SDA hold time is the delay from the falling edge of SCL to the
		 * changing of SDA
		 */
		uint16_t min_sda_hold_time_ns = 0;
		/**
		 * The SCL start hold time is the delay from the falling edge of SDA
		 * while SCL is high (start condition) to the falling edge of SCL
		 */
		uint16_t min_scl_start_hold_time_ns = 0;
		/**
		 * The SCL stop hold time is the delay from the rising edge of SCL to
		 * the rising edge of SDA while SCL is high (stop condition)
		 */
		uint16_t min_scl_stop_hold_time_ns = 0;

		bool is_high_drive_mode = false;
	};

	explicit I2cMaster(const Config &config);
	explicit I2cMaster(nullptr_t);
	I2cMaster(const I2cMaster&) = delete;
	I2cMaster(I2cMaster &&rhs);
	~I2cMaster();

	I2cMaster& operator=(const I2cMaster&) = delete;
	I2cMaster& operator=(I2cMaster &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	bool GetByte(const Byte slave_addr, const Byte reg_addr, Byte *out_byte) override;
	std::vector<Byte> GetBytes(const Byte slave_addr, const Byte reg_addr, const uint8_t size) override;
	void GetBytes(const Byte slave_addr, const uint16_t reg_addr, Byte *buf, const uint8_t size) override;
	bool SendByte(const Byte slave_addr, const Byte reg_addr, const Byte byte) override;
	bool SendBytes(const Byte slave_addr, const Byte reg_addr, const Byte *bytes, const size_t size) override;
	bool SendBytes(const Byte slave_addr, const uint16_t reg_addr, const Byte *bytes, const uint8_t size) override;

private:
	bool InitModule(const Pin::Name scl_pin, const Pin::Name sda_pin);
	void InitPin(const Pin::Name scl_pin, const Pin::Name sda_pin);
	void InitC2Reg(const Config &config);
	void InitSmbReg();
	void InitSltReg(const Config &config);
	void InitBaudRate(const Config &config);

	void Uninit();

	void Start();
	void RepeatStart();
	void Stop();
	void ResetI2C();
	bool SendByte_(const Byte byte);
	bool ReadByte_(const bool is_last_byte, Byte *out_byte);

	uint8_t m_module;

	Pin* m_scl;
	Pin* m_sda;

	Config m_config;

	bool m_is_use_repeated_start;

	bool m_is_init;
};

}
}
