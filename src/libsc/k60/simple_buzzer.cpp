/*
 * simple_buzzer.cpp
 * Simple Buzzer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
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
inline PinConfig::Name GetPin(const uint8_t)
{
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

SimpleBuzzer::SimpleBuzzer(const uint8_t id)
		: m_pin(GetGpoConfig(id))
{}

void SimpleBuzzer::SetBeep(const bool is_beep)
{
	m_pin.Set(is_beep);
}

#else
SimpleBuzzer::SimpleBuzzer(const uint8_t)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use SimpleBuzzer");
}
void SimpleBuzzer::SetBeep(const bool) {}

#endif /* LIBSC_USE_BUZZER */

}
}
