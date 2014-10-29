/*
 * simple_buzzer.cpp
 * Simple Buzzer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"

#include "libsc/config.h"
#include "libsc/k60/simple_buzzer.h"

using namespace libbase::k60;

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
		: m_pin(GetGpoConfig(config.id)),
		  m_is_active_high(config.is_active_high)
{}

void SimpleBuzzer::SetBeep(const bool is_beep)
{
	m_pin.Set(is_beep ^ m_is_active_high);
}

bool SimpleBuzzer::GetBeep() const
{
	return !(m_pin.Get() ^ m_is_active_high);
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
