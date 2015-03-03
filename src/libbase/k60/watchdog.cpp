/*
 * watchdog.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include <functional>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/vectors.h"
#include "libbase/k60/watchdog.h"
#include "libbase/k60/watchdog_c.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace k60
{

namespace
{

inline void Unlock()
{
	__disable_irq();
	WDOG->UNLOCK = 0xC520;
	WDOG->UNLOCK = 0xD928;
	__enable_irq();
}

}

void Watchdog::Init()
{
	Unlock();

	const Config &config = GetWatchdogConfig();
	if (config.is_enable)
	{
		InitTimeOutReg(config);
		InitPrescReg();
	}
	InitStctrlReg(config);
}

void Watchdog::InitStctrlReg(const Config &config)
{
	SET_BIT(WDOG->STCTRLL, WDOG_STCTRLL_INTFLG_SHIFT);

	uint16_t reg_h = 0;
#if MK60DZ10 || MK60D10
	SET_BIT(reg_h, WDOG_STCTRLH_STNDBYEN_SHIFT);
#endif
	SET_BIT(reg_h, WDOG_STCTRLH_WAITEN_SHIFT);
	SET_BIT(reg_h, WDOG_STCTRLH_STOPEN_SHIFT);
	SET_BIT(reg_h, WDOG_STCTRLH_ALLOWUPDATE_SHIFT);
	if (config.is_enable)
	{
		if (config.isr)
		{
			//SetIsr(Watchdog_IRQn, config.isr);
			SET_BIT(reg_h, WDOG_STCTRLH_IRQRSTEN_SHIFT);
			//EnableIrq(Watchdog_IRQn);
		}
		SET_BIT(reg_h, WDOG_STCTRLH_WDOGEN_SHIFT);
	}

	WDOG->STCTRLH = reg_h;
}

void Watchdog::InitTimeOutReg(const Config &config)
{
	assert(config.time_out_ms >= 2);

	WDOG->TOVALH = (config.time_out_ms & 0xFFFF0000) >> 16;
	WDOG->TOVALL = config.time_out_ms;
}

void Watchdog::InitPrescReg()
{
	WDOG->PRESC = 0;
}

void Watchdog::Refresh()
{
	__disable_irq();
	WDOG->REFRESH = 0xA602;
	WDOG->REFRESH = 0xB480;
	__enable_irq();
}

Watchdog::Config Watchdog::GetWatchdogConfig()
{
	return {};
}

}
}

void LibbaseK60WatchdogInit()
{
	libbase::k60::Watchdog::Init();
}
