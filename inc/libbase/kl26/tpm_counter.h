/*
 * tpm_counter.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/kl26/pin.h"

namespace libbase
{
namespace kl26
{

class TpmCounter
{
public:
	struct Config
	{
		uint8_t module;
		Pin::Name clockin_pin;
	};

	explicit TpmCounter(const Config &config);
	explicit TpmCounter(nullptr_t);
	TpmCounter(const TpmCounter&) = delete;
	TpmCounter(TpmCounter &&rhs);
	~TpmCounter();

	TpmCounter& operator=(const TpmCounter&) = delete;
	TpmCounter& operator=(TpmCounter &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void Update();

	uint16_t GetCount() const
	{
		return m_count;
	}

private:
	bool InitSim(const Pin::Name pin);
	bool InitClockPin(const Pin::Name pin);

	void Uninit();

	uint8_t m_module;
	uint16_t m_count;

	Pin m_clkin;

	bool m_is_init;
};

}
}
