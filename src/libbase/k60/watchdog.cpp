/*
 * watchdog.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/misc_utils.h"
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

Watchdog* g_instance = nullptr;

}

Watchdog::Watchdog(const Config &config)
{
	// FIXME Always reset here
	Unlock();
	InitTimeOutReg(config);
	InitPrescReg(config);
	InitStctrlReg(config);

	g_instance = this;
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
	if (config.is_allow_update)
	{
		SET_BIT(reg_h, WDOG_STCTRLH_ALLOWUPDATE_SHIFT);
	}
	if (config.isr)
	{
		m_isr = config.isr;

		SetIsr(Watchdog_IRQn, IrqHandler);
		SET_BIT(reg_h, WDOG_STCTRLH_IRQRSTEN_SHIFT);
		EnableIrq(Watchdog_IRQn);
	}
	SET_BIT(reg_h, WDOG_STCTRLH_CLKSRC_SHIFT);
	if (config.is_enable)
	{
		SET_BIT(reg_h, WDOG_STCTRLH_WDOGEN_SHIFT);
	}

	WDOG->STCTRLH = reg_h;
}

void Watchdog::InitTimeOutReg(const Config &config)
{
	assert(config.time_out_ms >= 50 && config.time_out_ms <= 8000);

	const uint32_t time_out_ms_ = Clamp<uint32_t>(50, config.time_out_ms, 8000);
	// We'll be using prescaler for values > 1000
	uint32_t tick;
	if (time_out_ms_ <= 4000)
	{
		tick = ClockUtils::GetBusTickPerMs(time_out_ms_)
				/ ((time_out_ms_ + 999) / 1000);
	}
	else
	{
		tick = ClockUtils::GetBusTickPerMs(time_out_ms_
				/ ((time_out_ms_ + 999) / 1000));
	}
	WDOG->TOVALH = tick >> 16;
	WDOG->TOVALL = tick & 0xFFFF;
}

void Watchdog::InitPrescReg(const Config &config)
{
	const uint32_t time_out_ms_ = Clamp<uint32_t>(50, config.time_out_ms, 8000);
	WDOG->PRESC = WDOG_PRESC_PRESCVAL((time_out_ms_ - 1) / 1000);
}

void Watchdog::StartupInitialize()
{
	Unlock();

	// Disable but allow reconfig on startup
	uint16_t reg_h = 0;
	SET_BIT(reg_h, WDOG_STCTRLH_DISTESTWDOG_SHIFT);
#if MK60DZ10 || MK60D10
	SET_BIT(reg_h, WDOG_STCTRLH_STNDBYEN_SHIFT);
#endif
	SET_BIT(reg_h, WDOG_STCTRLH_WAITEN_SHIFT);
	SET_BIT(reg_h, WDOG_STCTRLH_STOPEN_SHIFT);
	SET_BIT(reg_h, WDOG_STCTRLH_ALLOWUPDATE_SHIFT);
	SET_BIT(reg_h, WDOG_STCTRLH_CLKSRC_SHIFT);

	WDOG->STCTRLH = reg_h;
}

void Watchdog::Refresh()
{
	__disable_irq();
	WDOG->REFRESH = 0xA602;
	WDOG->REFRESH = 0xB480;
	__enable_irq();
}

__ISR void Watchdog::IrqHandler()
{
	if (!g_instance)
	{
		// Something's wrong?
		assert(false);
		return;
	}

	if (g_instance->m_isr)
	{
		g_instance->m_isr();
	}
}

}
}

void LibbaseK60WatchdogStartupInitialize()
{
	libbase::k60::Watchdog::StartupInitialize();
}
