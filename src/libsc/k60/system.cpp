/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/watchdog.h"

#include "libsc/config.h"
#include "libsc/k60/system.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

System *System::m_instance = nullptr;

System::System()
		: //m_watchdog(GetWatchdogConfig()),
		  m_delay(),
		  m_timer()
{}

__attribute__((__weak__)) Watchdog::Config System::GetWatchdogConfig()
{
	return {};
}

}
}
