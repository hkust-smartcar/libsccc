/*
 * simple_buzzer.cpp
 * Simple Buzzer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"

#include "libsc/config.h"
#include "libsc/k60/simple_buzzer.h"

using namespace libbase::k60;

// Default to active high
#ifndef LIBSC_BUZZER_ACIVE_LEVEL
	#define LIBSC_BUZZER_ACIVE_LEVEL 1
#endif

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_BUZZER

namespace
{

#if LIBSC_USE_BUZZER == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_BUZZER0;
}

#endif

Gpo::Config GetGpoConfig(const uint8_t id)
{
	Gpo::Config config;
	config.pin = GetPin(id);
	return config;
}

}

SimpleBuzzer::SimpleBuzzer(const Config &config)
		: m_pin(GetGpoConfig(config.id))
{}

void SimpleBuzzer::SetBeep(const bool is_beep)
{
	m_pin.Set(!(is_beep ^ LIBSC_BUZZER_ACIVE_LEVEL));
}

bool SimpleBuzzer::GetBeep() const
{
	return !(m_pin.Get() ^ LIBSC_BUZZER_ACIVE_LEVEL);
}

#else
SimpleBuzzer::SimpleBuzzer(const Config&)
		: m_pin(nullptr)
{
	LOG_DL("Configured not to use SimpleBuzzer");
}
void SimpleBuzzer::SetBeep(const bool) {}
bool SimpleBuzzer::GetBeep() const { return false; }

#endif /* LIBSC_USE_BUZZER */

}
}
