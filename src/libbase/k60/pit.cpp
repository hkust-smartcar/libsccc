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

}

Pit::Pit(const PitConfig &config, OnPitTriggerListener isr)
		: m_channel(config.channel)
{
	assert(m_channel < 4);
	assert(!g_instances[m_channel]);
	assert(config.count > 0);
	g_instances[m_channel] = this;

	// Enable PIT
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	CLEAR_BIT(PIT_MCR, PIT_MCR_MDIS_SHIFT);
	CLEAR_BIT(PIT_MCR, PIT_MCR_FRZ_SHIFT);

	SetEnable(false);
	SetCount(config.count);
	ConsumeInterrupt(m_channel);
	SetEnable(true);
	SetIsr(isr);
}

Pit::Pit(const PitConfig &config)
		: Pit(config, nullptr)
{}

Pit::~Pit()
{
	SetIsr(nullptr);
	SetEnable(false);
	g_instances[m_channel] = nullptr;
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

void Pit::SetIsr(OnPitTriggerListener isr)
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

__ISR void Pit::IrqHandler()
{
	const VECTORn_t v = GetVectorX();
	const int channel = v - PIT0_VECTORn;
	if (g_instances[channel])
	{
		g_instances[channel]->m_isr(g_instances[channel]);
	}
	ConsumeInterrupt(channel);
}

}
}
