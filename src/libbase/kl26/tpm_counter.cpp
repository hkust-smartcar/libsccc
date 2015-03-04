/*
 * tpm_counter.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/tpm_counter.h"
#include "libbase/kl26/tpm_utils.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace kl26
{

namespace
{

constexpr TPM_Type* MEM_MAPS[PINOUT::GetTpmCount()] =
{
	TPM0,
	TPM1,
	TPM2,
};

}

TpmCounter::TpmCounter(const Config &config)
		: m_module(config.module),
		  m_count(0),
		  m_clkin(nullptr),
		  m_is_init(false)
{
	if (!Tpm::Get().RegTpm(TpmUtils::GetTpm(m_module, 0), true)
			|| !InitSim(config.clockin_pin) || !InitClockPin(config.clockin_pin))
	{
		assert(false);
		return;
	}

	m_is_init = true;

	Sim::SetTpmClockSource(Sim::TpmClock::kPll);
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kTpm0, m_module), true);

	MEM_MAPS[m_module]->SC = 0;
	MEM_MAPS[m_module]->CNT = 0;
	MEM_MAPS[m_module]->MOD = TPM_MOD_MOD(0xFFFF);
	MEM_MAPS[m_module]->CONF = 0;

	// Select clock and enable the counter
	MEM_MAPS[m_module]->SC |= TPM_SC_CMOD(2);
}

TpmCounter::TpmCounter(nullptr_t)
		: m_module(0),
		  m_count(0),
		  m_clkin(false),
		  m_is_init(false)
{}

TpmCounter::~TpmCounter()
{
	Uninit();
}

TpmCounter& TpmCounter::operator=(TpmCounter &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_module = rhs.m_module;
			m_count = rhs.m_count;

			m_clkin = std::move(rhs.m_clkin);

			m_is_init = true;
		}
	}
	return *this;
}

bool TpmCounter::InitSim(const Pin::Name pin)
{
	Tpm::ClkinName tpm = PINOUT::GetTpmClkin(pin);
	if (tpm == Tpm::ClkinName::kDisable)
	{
		return false;
	}

	Sim::SetTpmExternalClockPin(m_module, static_cast<Uint>(tpm));
	return true;
}

bool TpmCounter::InitClockPin(const Pin::Name pin)
{
	if (PINOUT::GetTpmClkin(pin) == Tpm::ClkinName::kDisable)
	{
		return false;
	}

	Pin::Config config;
	config.pin = pin;
	config.mux = PINOUT::GetTpmClkinMux(pin);

	m_clkin = Pin(config);
	return true;
}

void TpmCounter::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		m_clkin = Pin(nullptr);
		MEM_MAPS[m_module]->SC = 0;
		if (Tpm::Get().UnregTpm(TpmUtils::GetTpm(m_module, 0)))
		{
			Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kTpm0, m_module),
					false);
		}
	}
}

void TpmCounter::Update()
{
	m_count = MEM_MAPS[m_module]->CNT;
	MEM_MAPS[m_module]->CNT = 0;
}

}
}
