/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libsc/com/config.h"
#include "libsc/k60/pit_timer.h"
#include "libsc/k60/sys_tick_delay.h"
#include "libsc/k60/system.h"

namespace libsc
{
namespace k60
{

System *System::m_instance = nullptr;

System::System()
		: m_timer(LIBSC_SYSTEM_TIMER_PIT_CHANNEL)
{}

}
}
