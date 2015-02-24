/*
 * soft_i2c_master.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <vector>

#include "libbase/log.h"
#include "libbase/k60/dwt.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/i2c_master_interface.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/soft_i2c_master.h"

#include "libsc/k60/system.h"

using namespace std;
using namespace libsc::k60;

#define SEND_BYTE_GUARDED(x, ret) do { if (!SendByte_(x)) { Stop(); return ret; } } while (false)

#define IS_I2C_HIGH(x) (!x.IsOutput())

namespace libbase
{
namespace k60
{

namespace
{

Uint GetDelayUs(const SoftI2cMaster::Config &config)
{
	const Uint freq2 = config.freq_khz * 2;
	return (1000 + freq2 - 1) / freq2;
}

Gpi::Config GetSclConfig(const SoftI2cMaster::Config &config)
{
	Gpi::Config gc;
	gc.pin = config.scl_pin;
	gc.config[Pin::Config::ConfigBit::kOpenDrain] = true;
	return gc;
}

Gpi::Config GetSdaConfig(const SoftI2cMaster::Config &config)
{
	Gpi::Config gc;
	gc.pin = config.sda_pin;
	gc.config[Pin::Config::ConfigBit::kOpenDrain] = true;
	return gc;
}

}

inline void SoftI2cMaster::Delay()
{
	Dwt::DelayUs(m_delay_us);
}

SoftI2cMaster::SoftI2cMaster(const Config &config)
		: m_delay_us(GetDelayUs(config)),
		  m_scl_low_timeout_ms((config.scl_low_timeout * m_delay_us + 999)
				/ 1000),
		  m_scl(GetSclConfig(config)),
		  m_sda(GetSdaConfig(config)),
		  m_is_init(true)
{}

SoftI2cMaster::SoftI2cMaster(SoftI2cMaster &&rhs)
		: SoftI2cMaster(nullptr)
{
	*this = std::move(rhs);
}

SoftI2cMaster::SoftI2cMaster(nullptr_t)
		: m_delay_us(0),
		  m_scl_low_timeout_ms(0),
		  m_scl(nullptr),
		  m_sda(nullptr),
		  m_is_init(false)
{}

SoftI2cMaster::~SoftI2cMaster()
{
	Uninit();
}

SoftI2cMaster& SoftI2cMaster::operator=(SoftI2cMaster &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_delay_us = rhs.m_delay_us;
			m_scl_low_timeout_ms = rhs.m_scl_low_timeout_ms;

			m_scl = std::move(rhs.m_scl);
			m_sda = std::move(rhs.m_sda);

			m_is_init = true;
		}
	}
	return *this;
}

void SoftI2cMaster::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		m_scl = Gpio(nullptr);
		m_sda = Gpio(nullptr);
	}
}

void SoftI2cMaster::Start()
{
	if (!IS_I2C_HIGH(m_sda))
	{
		if (IS_I2C_HIGH(m_scl))
		{
			m_sda.EnsureGpi();
			Delay();
		}
		else
		{
			m_sda.EnsureGpi();
			//Delay();
			m_scl.EnsureGpi();
			Delay();
		}
	}
	else
	{
		m_scl.EnsureGpi();
		Delay();
	}

	m_sda.Clear();
	//Delay();
	m_scl.Clear();
	Delay();
}

void SoftI2cMaster::Stop()
{
	if (IS_I2C_HIGH(m_sda))
	{
		if (IS_I2C_HIGH(m_scl))
		{
			m_scl.Clear();
			Delay();
			m_sda.Clear();
			//Delay();
			m_scl.EnsureGpi();
			Delay();
		}
		else
		{
			m_sda.Clear();
			//Delay();
			m_scl.EnsureGpi();
			Delay();
		}
	}
	else
	{
		m_scl.EnsureGpi();
		Delay();
	}

	m_sda.EnsureGpi();
	//Delay();
}

// Assume SCL is low when being called
bool SoftI2cMaster::SendByte_(const Byte byte)
{
	for (Uint i = 0; i < 8; ++i)
	{
		if ((byte >> (7 - i)) & 0x1)
		{
			m_sda.EnsureGpi();
		}
		else
		{
			m_sda.Clear();
		}
		m_scl.EnsureGpi();
		Delay();
		m_scl.Clear();
		Delay();
	}

	m_sda.EnsureGpi();
	//Delay();
	m_scl.EnsureGpi();
	Delay();
	const bool ack = !m_sda.Get();
	m_scl.Clear();
	Delay();
	// Pull SDA low in order to be consistent no matter ACK succeeded or not
	m_sda.Clear();
	//Delay();
	return ack;
}

// Assume SCL is low when being called
bool SoftI2cMaster::ReadByte_(const bool is_ack, Byte *out_byte)
{
	m_sda.EnsureGpi();

	Byte byte = 0;
	for (Uint i = 0; i < 8; ++i)
	{
		m_scl.EnsureGpi();
		Delay();

		// TODO libbase should not depends on libsc
		const Timer::TimerInt start = System::Time();
		// Clock stretching
		while (!m_scl.Get())
		{
			if (Timer::TimeDiff(System::Time(), start) >= m_scl_low_timeout_ms)
			{
				LOG_DL("i2c scl timeout");
				return false;
			}
		}

		byte |= m_sda.Get() << (7 - i);
		m_scl.Clear();
		Delay();
	}

	if (is_ack)
	{
		m_sda.Clear();
	}
	//Delay();
	m_scl.EnsureGpi();
	Delay();
	m_scl.Clear();
	Delay();
	*out_byte = byte;
	return true;
}

bool SoftI2cMaster::GetByte(const Byte slave_addr, const Byte reg_addr,
		Byte *out_byte)
{
	STATE_GUARD(SoftI2cMaster, false);

	Start();
	SEND_BYTE_GUARDED(slave_addr << 1, false);
	SEND_BYTE_GUARDED(reg_addr, false);
	Start();
	SEND_BYTE_GUARDED((slave_addr << 1) | 0x1, false);
	if (!ReadByte_(false, out_byte))
	{
		Stop();
		return false;
	}
	else
	{
		Stop();
		return true;
	}
}

vector<Byte> SoftI2cMaster::GetBytes(const Byte slave_addr, const Byte reg_addr,
		const uint8_t size)
{
	STATE_GUARD(SoftI2cMaster, {});

	vector<Byte> bytes;
	bytes.reserve(size);

	Start();
	SEND_BYTE_GUARDED((slave_addr << 1) & 0xFE, {});
	SEND_BYTE_GUARDED(reg_addr, {});
	Start();
	SEND_BYTE_GUARDED(((slave_addr << 1) & 0xFE) | 0x1, {});
	for (uint8_t i = 0; i < size; ++i)
	{
		Byte byte;
		if (!ReadByte_(true, &byte))
		{
			Stop();
			return bytes;
		}
		bytes.push_back(byte);
	}
	Stop();
	return bytes;
}

bool SoftI2cMaster::SendByte(const Byte slave_addr, const Byte reg_addr,
		const Byte byte)
{
	STATE_GUARD(SoftI2cMaster, false);

	Start();
	SEND_BYTE_GUARDED((slave_addr << 1) & 0xFE, false);
	SEND_BYTE_GUARDED(reg_addr, false);
	SEND_BYTE_GUARDED(byte, false);
	Stop();
	return true;
}

bool SoftI2cMaster::SendBytes(const Byte slave_addr, const Byte reg_addr,
		const Byte *bytes, const size_t size)
{
	STATE_GUARD(SoftI2cMaster, false);

	Start();
	SEND_BYTE_GUARDED((slave_addr << 1) & 0xFE, false);
	SEND_BYTE_GUARDED(reg_addr, false);
	for (size_t i = 0; i < size; ++i)
	{
		SEND_BYTE_GUARDED(bytes[i], false);
	}
	Stop();
	return true;
}

}
}
