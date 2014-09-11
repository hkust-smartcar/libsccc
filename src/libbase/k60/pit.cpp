/*
 * pit.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include <algorithm>
#include <functional>

#include "libbase/log.h"
#include "libbase/k60/clock_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pit.h"
#include "libbase/k60/sim.h"
#include "libbase/k60/vectors.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace k60
{

namespace
{

inline void ConsumeInterrupt_(const int channel)
{
	SET_BIT(PIT->CHANNEL[channel].TFLG, PIT_TFLG_TIF_SHIFT);
}

Pit* g_instances[4] = {};

}

Pit::Pit(const Config &config)
		: m_channel(config.channel), m_is_init(true)
{
	assert(m_channel < 4);
	assert(!g_instances[m_channel]);
	assert(config.count > 0);
	g_instances[m_channel] = this;

	// System enable PIT
	Sim::SetEnableClockGate(Sim::ClockGate::kPit, true);
	CLEAR_BIT(PIT->MCR, PIT_MCR_MDIS_SHIFT);
	CLEAR_BIT(PIT->MCR, PIT_MCR_FRZ_SHIFT);

	SetEnable(false);
	SetCount(config.count);
	ConsumeInterrupt();
	SetIsr(config.isr);
	if (config.is_enable)
	{
		SetEnable(true);
	}
}

Pit::Pit(Pit &&rhs)
		: Pit(nullptr)
{
	*this = std::move(rhs);
}

Pit::Pit(nullptr_t)
		: m_channel(0),
		  m_is_init(false)
{}

Pit::~Pit()
{
	Uninit();
}

Pit& Pit::operator=(Pit &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_channel = rhs.m_channel;
			// Copy instead of move to prevent race condition
			m_isr = rhs.m_isr;

			m_is_init = true;
			g_instances[m_channel] = this;
		}
	}
	return *this;
}

void Pit::SetIsr(const OnPitTriggerListener &isr)
{
	m_isr = isr;
	if (m_isr)
	{
		::SetIsr(EnumAdvance(PIT0_IRQn, m_channel), IrqHandler);
		SET_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TIE_SHIFT);
		EnableIrq(EnumAdvance(PIT0_IRQn, m_channel));
	}
	else
	{
		DisableIrq(EnumAdvance(PIT0_IRQn, m_channel));
		::SetIsr(EnumAdvance(PIT0_IRQn, m_channel), nullptr);
		CLEAR_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TIE_SHIFT);
	}
}

void Pit::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		SetIsr(nullptr);
		SetEnable(false);
		g_instances[m_channel] = nullptr;
	}
}

void Pit::SetEnable(const bool flag)
{
	STATE_GUARD(Pit, VOID);

	if (flag)
	{
		SET_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TEN_SHIFT);
	}
	else
	{
		CLEAR_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TEN_SHIFT);
	}
}

void Pit::SetCount(const uint32_t count)
{
	STATE_GUARD(Pit, VOID);

	PIT->CHANNEL[m_channel].LDVAL = std::max<uint32_t>(count, 1) - 1;
	if (GET_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TEN_SHIFT))
	{
		SetEnable(false);
		SetEnable(true);
	}
}

uint32_t Pit::GetCountLeft() const
{
	STATE_GUARD(Pit, 0);

	return PIT->CHANNEL[m_channel].CVAL;
}

void Pit::ConsumeInterrupt()
{
	STATE_GUARD(Pit, VOID);

	ConsumeInterrupt_(m_channel);
}

bool Pit::IsInterruptRequested() const
{
	STATE_GUARD(Pit, false);

	return GET_BIT(PIT->CHANNEL[m_channel].TFLG, PIT_TFLG_TIF_SHIFT);
}

__ISR void Pit::IrqHandler()
{
	const int channel = GetActiveIrq() - PIT0_IRQn;
	Pit *const that = g_instances[channel];
	if (!that || !(*that))
	{
		// Something's wrong?
		assert(false);
		ConsumeInterrupt_(channel);
		return;
	}

	if (that->m_isr)
	{
		that->m_isr(that);
	}
	ConsumeInterrupt_(channel);
}

}
}
