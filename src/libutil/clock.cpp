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

#include <vectors.h>
#include <MK60_pit.h>

#include "libutil/clock.h"

#define PRECISION 1

namespace libutil
{

volatile Clock::ClockInt Clock::m_ms = 0;

void Clock::Init()
{
	SetIsr(PIT0_VECTORn, IrqHandler);
	pit_init_ms(PIT0, PRECISION);
	EnableIsr(PIT0_VECTORn);
}

void Clock::IrqHandler()
{
	m_ms += PRECISION;
	PIT_Flag_Clear(PIT0);
}

}
