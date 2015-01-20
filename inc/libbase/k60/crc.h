/*
 * crc.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class Crc
{
public:
	struct Config
	{
		uint32_t polynomial = 0x04C11DB7;
		uint32_t seed = 0xFFFFFFFF;
	};

	explicit Crc(const Config &config);
	explicit Crc(nullptr_t);
	Crc(const Crc&) = delete;
	Crc(Crc &&rhs);
	~Crc();

	Crc& operator=(const Crc&) = delete;
	Crc& operator=(Crc &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void NewInstance(const uint32_t seed);
	void NewInstance()
	{
		NewInstance(m_seed);
	}

	void PushData(const uint32_t data);
	void PushData(const Byte data);

	uint32_t GetCrc() const;

private:
	void Uninit();

	uint32_t m_seed;

	bool m_is_init;
};

}
}
