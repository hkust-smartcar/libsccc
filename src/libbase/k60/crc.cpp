/*
 * crc.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cstddef>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/crc.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/sim.h"

namespace libbase
{
namespace k60
{

Crc::Crc(const Config &config)
		: m_seed(config.seed), m_is_init(true)
{
	Sim::SetEnableClockGate(Sim::ClockGate::kCrc, true);

	NewInstance(config.seed);
	CRC0->CTRL = 0 | CRC_CTRL_TCRC_MASK | CRC_CTRL_TOT(1) | CRC_CTRL_TOTR(1)
			| CRC_CTRL_FXOR_MASK;
	CRC0->GPOLY = config.polynomial;
}

Crc::Crc(Crc &&rhs)
		: Crc(nullptr)
{
	*this = std::move(rhs);
}

Crc::Crc(nullptr_t)
		: m_seed(0), m_is_init(false)
{}

Crc::~Crc()
{
	Uninit();
}

Crc& Crc::operator=(Crc &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_seed = rhs.m_seed;

			m_is_init = true;
		}
	}
	return *this;
}

void Crc::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		Sim::SetEnableClockGate(Sim::ClockGate::kCrc, false);
	}
}

void Crc::NewInstance(const uint32_t seed)
{
	STATE_GUARD(Crc, VOID);

	SET_BIT(CRC0->CTRL, CRC_CTRL_WAS_SHIFT);
	CRC0->CRC = seed;
	CLEAR_BIT(CRC0->CTRL, CRC_CTRL_WAS_SHIFT);
	m_seed = seed;
}

void Crc::PushData(const uint32_t data)
{
	STATE_GUARD(Crc, VOID);

	CRC0->CRC = data;
}

void Crc::PushData(const Byte data)
{
	STATE_GUARD(Crc, VOID);

	CRC0->ACCESS8BIT.CRCLL = data;
}

uint32_t Crc::GetCrc() const
{
	STATE_GUARD(Crc, 0);

	return CRC0->CRC;
}

}
}
