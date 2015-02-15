/*
 * sys_tick.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
/*

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/sys_tick.h"
#include "libbase/kl26/vectors.h"

#include "libutil/misc.h"

#define SYST ((SysTick_Type*)SysTick_BASE)
#define SYST_CSR (SYST->CTRL)
#define SYST_RVR (SYST->LOAD)
#define SYST_CVR (SYST->VAL)
#define SYST_CALIB (SYST->CALIB)

#define SYST_CSR_COUNTFLAG_SHIFT	SysTick_CTRL_COUNTFLAG_Pos
#define SYST_CSR_CLKSOURCE_MASK		SysTick_CTRL_CLKSOURCE_Msk
#define SYST_CSR_CLKSOURCE_SHIFT	SysTick_CTRL_CLKSOURCE_Pos
#define SYST_CSR_TICKINT_MASK		SysTick_CTRL_TICKINT_Msk
#define SYST_CSR_TICKINT_SHIFT		SysTick_CTRL_TICKINT_Pos
#define SYST_CSR_ENABLE_MASK		SysTick_CTRL_ENABLE_Msk
#define SYST_CSR_ENABLE_SHIFT		SysTick_CTRL_ENABLE_Pos

#define SYST_RVR_RELOAD_MASK		SysTick_LOAD_RELOAD_Msk
#define SYST_RVR_RELOAD_SHIFT		SysTick_LOAD_RELOAD_Pos
#define SYST_RVR_RELOAD(x)			(((uint32_t)(((uint32_t)(x))<<SYST_RVR_RELOAD_SHIFT))&SYST_RVR_RELOAD_MASK)

using namespace libutil;

namespace libbase
{
namespace kl26
{

namespace
{

inline void ConsumeInterrupt_()
{
	if (GET_BIT(SYST_CSR, SYST_CSR_COUNTFLAG_SHIFT))
	{
		SYST_CVR = 0;
	}
}

SysTick* g_instance = nullptr;

}

SysTick::SysTick(const Config &config)
		: m_is_init(true)
{
	assert(!g_instance);
	assert(config.count > 0);
	g_instance = this;

	SetEnable(false);
	InitCsrReg(config);
	SetCount(config.count);
	ConsumeInterrupt();
	SetIsr(config.isr);
	if (config.is_enable)
	{
		SetEnable(true);
	}
}

SysTick::SysTick(SysTick &&rhs)
		: SysTick(nullptr)
{
	*this = std::move(rhs);
}

SysTick::SysTick(nullptr_t)
		: m_is_init(false)
{}

SysTick::~SysTick()
{
	Uninit();
}

SysTick& SysTick::operator=(SysTick &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			// Copy instead of move to prevent race condition
			m_isr = rhs.m_isr;

			m_is_init = true;
			g_instance = this;
		}
	}
	return *this;
}

void SysTick::InitCsrReg(const Config&)
{
	uint32_t reg = 0;
	SET_BIT(reg, SYST_CSR_CLKSOURCE_SHIFT);

	SYST_CSR = reg;
}

void SysTick::SetIsr(const OnSysTickTriggerListener &isr)
{
	m_isr = isr;
	if (m_isr)
	{
		::SetIsr(SysTick_IRQn, IrqHandler);
		SET_BIT(SYST_CSR, SYST_CSR_TICKINT_SHIFT);
		//EnableIrq(SysTick_IRQn);
	}
	else
	{
		//DisableIrq(SysTick_IRQn);
		::SetIsr(SysTick_IRQn, nullptr);
		CLEAR_BIT(SYST_CSR, SYST_CSR_TICKINT_SHIFT);
	}
}

void SysTick::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		SetIsr(nullptr);
		SetEnable(false);
		g_instance = nullptr;
	}
}

void SysTick::SetEnable(const bool flag)
{
	STATE_GUARD(SysTick, VOID);

	if (flag)
	{
		SET_BIT(SYST_CSR, SYST_CSR_ENABLE_SHIFT);
	}
	else
	{
		CLEAR_BIT(SYST_CSR, SYST_CSR_ENABLE_SHIFT);
	}
}

void SysTick::SetCount(const uint32_t count)
{
	STATE_GUARD(SysTick, VOID);

	SYST_RVR = SYST_RVR_RELOAD(count);
	SYST_CVR = 0;
}

uint32_t SysTick::GetCountLeft() const
{
	STATE_GUARD(SysTick, 0);

	return SYST_CVR;
}

void SysTick::ConsumeInterrupt()
{
	STATE_GUARD(SysTick, VOID);

	ConsumeInterrupt_();
}

bool SysTick::IsInterruptRequested() const
{
	STATE_GUARD(SysTick, false);

	return GET_BIT(SYST_CSR, SYST_CSR_COUNTFLAG_SHIFT);
}

__ISR void SysTick::IrqHandler()
{
	SysTick *const that = g_instance;
	if (!g_instance || !(*g_instance))
	{
		// Something's wrong?
		assert(false);
		ConsumeInterrupt_();
		return;
	}

	if (that->m_isr)
	{
		that->m_isr(that);
	}
	ConsumeInterrupt_();
}

}
}
*/
