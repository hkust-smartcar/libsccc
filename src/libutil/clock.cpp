/*
 * clock.cpp
 * Clock util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <misc.h>
#include <vectors.h>
#include <MK60_pit.h>

#include "libutil/clock.h"

// We don't need a 1ms precision, at least for now
#define PRECISION 5

namespace libutil
{

volatile Clock::ClockInt Clock::m_ms = 0;

void Clock::Init()
{
	SetIsr(PIT0_VECTORn, IrqHandler);
	pit_init_ms(PIT0, PRECISION);
	enable_irq(PIT0_IRQn);
}

void Clock::IrqHandler()
{
	m_ms += PRECISION;
	PIT_Flag_Clear(PIT0);
}

}
