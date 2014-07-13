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

#include <vectors.h>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pit.h"
#include "libbase/k60/sim.h"

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
		if (rhs.m_is_init)
		{
			m_channel = rhs.m_channel;
			// Copy instead of move to prevent race condition
			m_isr = rhs.m_isr;

			m_is_init = true;
			g_instances[m_channel] = this;

			rhs.m_is_init = false;
		}
	}
	return *this;
}

void Pit::SetIsr(const OnPitTriggerListener &isr)
{
	m_isr = isr;
	if (m_isr)
	{
		::SetIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel), IrqHandler);
		SET_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TIE_SHIFT);
		EnableIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel));
	}
	else
	{
		DisableIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel));
		::SetIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel), nullptr);
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
	if (!*this)
	{
		return;
	}
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
	if (!*this)
	{
		return;
	}
	PIT->CHANNEL[m_channel].LDVAL = std::max<uint32_t>(count, 1) - 1;
	if (GET_BIT(PIT->CHANNEL[m_channel].TCTRL, PIT_TCTRL_TEN_SHIFT))
	{
		SetEnable(false);
		SetEnable(true);
	}
}

uint32_t Pit::GetCountLeft() const
{
	if (!*this)
	{
		return 0;
	}
	return PIT->CHANNEL[m_channel].CVAL;
}

void Pit::ConsumeInterrupt()
{
	if (!*this)
	{
		return;
	}
	ConsumeInterrupt_(m_channel);
}

bool Pit::IsInterruptRequested() const
{
	if (!*this)
	{
		return false;
	}
	return GET_BIT(PIT->CHANNEL[m_channel].TFLG, PIT_TFLG_TIF_SHIFT);
}

__ISR void Pit::IrqHandler()
{
	const VECTORn_t v = GetVectorX();
	const int channel = v - PIT0_VECTORn;
	if (!g_instances[channel])
	{
		// Something's wrong?
		assert(false);
		ConsumeInterrupt_(channel);
		return;
	}

	if (g_instances[channel]->m_isr)
	{
		g_instances[channel]->m_isr(g_instances[channel]);
	}
	ConsumeInterrupt_(channel);
}

}
}
