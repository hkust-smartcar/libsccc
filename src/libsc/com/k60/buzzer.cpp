/*
 * buzzer.cpp
 * Buzzer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/buzzer.h"

namespace libsc
{

#ifdef LIBSC_USE_BUZZER

namespace
{

#if LIBSC_USE_BUZZER == 1
#define GetGpio(x) LIBSC_BUZZER0
#endif

}

Buzzer::Buzzer(const uint8_t id)
		: m_id(id)
{
	gpio_init(GetGpio(m_id), GPO, 0);
}

void Buzzer::SetBeep(const bool is_beep)
{
	return gpio_set(GetGpio(m_id), is_beep ? 1 : 0);
}

#else
Buzzer::Buzzer(const uint8_t) : m_id(0) {}
void Buzzer::SetBeep(const bool is_beep) {}

#endif /* LIBSC_USE_BUZZER */

}
