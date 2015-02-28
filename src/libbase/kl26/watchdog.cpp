/*
 * watchdog.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstdint>

#include "libbase/kl26/watchdog.h"
#include "libbase/kl26/watchdog_c.h"

namespace libbase
{
namespace kl26
{

namespace
{

Watchdog* g_instance = nullptr;

}

Watchdog::Watchdog()
{}

void Watchdog::Init()
{
	const Config &config = GetWatchdogConfig();

	InitCopcReg(config);

	g_instance = this;
}

void Watchdog::InitCopcReg(const Config &config)
{
	uint32_t reg = 0;
	reg |= SIM_COPC_COPT(static_cast<int>(config.timeout));

	SIM->COPC = reg;
}

void Watchdog::Refresh()
{
	__disable_irq();
	SIM->SRVCOP = SIM_SRVCOP_SRVCOP(0x55);
	SIM->SRVCOP = SIM_SRVCOP_SRVCOP(0xAA);
	__enable_irq();
}

}
}

void LibbaseKl26WatchdogInit()
{
	libbase::kl26::Watchdog::Get().Init();
}
