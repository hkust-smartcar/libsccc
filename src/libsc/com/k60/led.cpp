/*
 * led.cpp
 * LED abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/led.h"

namespace libsc
{

#ifdef LIBSC_USE_LED

Led::Led(const uint8_t id)
{
	m_id = id;
	gpio_init((PTXn_e)(LIBSC_LED0 + id), GPO, 1);
}

void Led::SetEnable(const bool flag)
{
	gpio_set((PTXn_e)(LIBSC_LED0 + m_id), flag ? 0 : 1);
}

#else
Led::Led(const uint8_t id) {}
void Led::SetEnable(const bool flag) {}

#endif

}
