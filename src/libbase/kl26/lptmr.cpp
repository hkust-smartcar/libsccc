/*
 * lptmr.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/lptmr.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/vectors.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace kl26
{

namespace
{

inline void ConsumeInterrupt_()
{
	SET_BIT(LPTMR0->CSR, LPTMR_CSR_TCF_SHIFT);
}

Lptmr* g_instance = nullptr;

}

Lptmr::Lptmr(const Config &config)
		: m_is_init(true)
{
	assert(!g_instance);
	assert(config.count > 0);
	g_instance = this;

	Sim::SetEnableClockGate(Sim::ClockGate::kLptimer, true);

	SetEnable(false);
	InitCsrReg(config);
	InitPsrReg();
	SetCount(config.count);
	SetIsr(config.isr);
	if (config.is_enable)
	{
		SetEnable(true);
	}
}

Lptmr::Lptmr(Lptmr &&rhs)
		: Lptmr(nullptr)
{
	*this = std::move(rhs);
}

Lptmr::Lptmr(nullptr_t)
		: m_is_init(false)
{}

Lptmr::~Lptmr()
{
	Uninit();
}

Lptmr& Lptmr::operator=(Lptmr &&rhs)
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

void Lptmr::InitCsrReg(const Config &config)
{
	uint32_t reg = 0;
	if (!config.is_reset_counter_on_irq)
	{
		SET_BIT(reg, LPTMR_CSR_TFC_SHIFT);
	}

	LPTMR0->CSR = reg;
}

void Lptmr::InitPsrReg()
{
	uint32_t reg = 0;
	SET_BIT(reg, LPTMR_PSR_PBYP_SHIFT);
	// Select 1kHz LPO
	reg |= LPTMR_PSR_PCS(1);

	LPTMR0->PSR = reg;
}

void Lptmr::SetIsr(const OnLptmrTriggerListener &isr)
{
	m_isr = isr;
	if (m_isr)
	{
		ConsumeInterrupt();
		::SetIsr(LPTimer_IRQn, IrqHandler);
		SET_BIT(LPTMR0->CSR, LPTMR_CSR_TIE_SHIFT);
		EnableIrq(LPTimer_IRQn);
	}
	else
	{
		DisableIrq(LPTimer_IRQn);
		::SetIsr(LPTimer_IRQn, nullptr);
		CLEAR_BIT(LPTMR0->CSR, LPTMR_CSR_TIE_SHIFT);
	}
}

void Lptmr::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		SetIsr(nullptr);
		SetEnable(false);
		g_instance = nullptr;

		Sim::SetEnableClockGate(Sim::ClockGate::kLptimer, false);
	}
}

void Lptmr::SetEnable(const bool flag)
{
	STATE_GUARD(Lptmr, VOID);

	if (flag)
	{
		SET_BIT(LPTMR0->CSR, LPTMR_CSR_TEN_SHIFT);
	}
	else
	{
		CLEAR_BIT(LPTMR0->CSR, LPTMR_CSR_TEN_SHIFT);
	}
}

void Lptmr::SetCount(const uint16_t count)
{
	STATE_GUARD(Lptmr, VOID);
	LPTMR0->CMR = LPTMR_CMR_COMPARE(count);
}

uint16_t Lptmr::GetCounter() const
{
	STATE_GUARD(Lptmr, 0);
	/*
	 * On each read of the CNR, software must first write to the CNR with any
	 * value
	 */
	LPTMR0->CNR = 0;
	return LPTMR0->CNR;
}

void Lptmr::ConsumeInterrupt()
{
	STATE_GUARD(Lptmr, VOID);
	ConsumeInterrupt_();
}

bool Lptmr::IsInterruptRequested() const
{
	STATE_GUARD(Lptmr, false);
	return GET_BIT(LPTMR0->CSR, LPTMR_CSR_TCF_SHIFT);
}

__ISR void Lptmr::IrqHandler()
{
	Lptmr *const that = g_instance;
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
