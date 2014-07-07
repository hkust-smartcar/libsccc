/*
 * pit.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <hw_common.h>

#include <cassert>
#include <cstdint>

#include <functional>

#include <vectors.h>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/delay.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pit.h"

namespace libbase
{
namespace k60
{

namespace
{

Pit* g_instances[4] = {};

inline void ConsumeInterrupt(const int channel)
{
	SET_BIT(PIT_TFLG(channel), PIT_TFLG_TIF_SHIFT);
}

Pit::Config GetPitConfig(const uint8_t channel)
{
	Pit::Config pc;
	pc.channel = channel;
	pc.is_enable = false;
	return pc;
}

}

Pit::Pit(const Config &config)
		: m_channel(config.channel), m_is_init(true)
{
	assert(m_channel < 4);
	assert(!g_instances[m_channel]);
	assert(config.count > 0);
	g_instances[m_channel] = this;

	// System enable PIT
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	CLEAR_BIT(PIT_MCR, PIT_MCR_MDIS_SHIFT);
	CLEAR_BIT(PIT_MCR, PIT_MCR_FRZ_SHIFT);

	SetEnable(false);
	SetCount(config.count);
	ConsumeInterrupt(m_channel);
	SetIsr(config.isr);
	if (config.is_enable)
	{
		SetEnable(true);
	}
}

Pit::Pit(nullptr_t)
		: m_channel(0), m_isr(nullptr), m_is_init(false)
{}

Pit::Pit(Pit &&rhs)
		: m_channel(rhs.m_channel), m_isr(std::move(rhs.m_isr)),
		  m_is_init(rhs.m_is_init)
{
	rhs.m_is_init = false;
}

Pit::~Pit()
{
	Uninit();
}

Pit& Pit::operator=(Pit &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		m_channel = rhs.m_channel;
		m_isr = std::move(rhs.m_isr);
		m_is_init = rhs.m_is_init;
		rhs.m_is_init = false;
	}
	return *this;
}

void Pit::SetIsr(const OnPitTriggerListener &isr)
{
	m_isr = isr;
	if (m_isr)
	{
		::SetIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel), IrqHandler);
		SET_BIT(PIT_TCTRL(m_channel), PIT_TCTRL_TIE_SHIFT);
		EnableIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel));
	}
	else
	{
		DisableIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel));
		::SetIsr(static_cast<VECTORn_t>(PIT0_VECTORn + m_channel), nullptr);
		CLEAR_BIT(PIT_TCTRL(m_channel), PIT_TCTRL_TIE_SHIFT);
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
	if (flag)
	{
		SET_BIT(PIT_TCTRL(m_channel), PIT_TCTRL_TEN_SHIFT);
	}
	else
	{
		CLEAR_BIT(PIT_TCTRL(m_channel), PIT_TCTRL_TEN_SHIFT);
	}
}

void Pit::SetCount(uint32_t count)
{
	PIT_LDVAL(m_channel) = count - 1;
	if (GET_BIT(PIT_TCTRL(m_channel), PIT_TCTRL_TEN_SHIFT))
	{
		SetEnable(false);
		SetEnable(true);
	}
}

uint32_t Pit::GetCountLeft() const
{
	return PIT_CVAL(m_channel);
}

bool Pit::IsInterruptRequested() const
{
	return GET_BIT(PIT_TFLG(m_channel), PIT_TFLG_TIF_SHIFT);
}

__ISR void Pit::IrqHandler()
{
	const VECTORn_t v = GetVectorX();
	const int channel = v - PIT0_VECTORn;
	if (!g_instances[channel])
	{
		// Something wrong?
		assert(false);
		ConsumeInterrupt(channel);
		return;
	}

	if (g_instances[channel]->m_isr)
	{
		g_instances[channel]->m_isr(g_instances[channel]);
	}
	ConsumeInterrupt(channel);
}

PitDelay::PitDelay(const uint8_t channel)
		: m_pit(GetPitConfig(channel))
{}

void PitDelay::DelayUs(const uint16_t us)
{
	m_pit.SetCount(us * ClockUtils::GetBusTickPerUs());
	m_pit.SetEnable(true);
	ConsumeInterrupt(m_pit.GetChannel());
	while (!m_pit.IsInterruptRequested())
	{}
}

void PitDelay::DelayMs(const uint16_t ms)
{
	m_pit.SetCount(ms * ClockUtils::GetBusTickPerMs());
	m_pit.SetEnable(true);
	ConsumeInterrupt(m_pit.GetChannel());
	while (!m_pit.IsInterruptRequested())
	{}
}

}
}
