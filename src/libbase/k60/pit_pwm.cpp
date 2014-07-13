/*
 * pit_pwm.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pit.h"
#include "libbase/k60/pit_pwm.h"

namespace libbase
{
namespace k60
{

namespace
{

Pit::Config GetPitConfig(const PitPwm::Config &config,
		const Pit::OnPitTriggerListener &isr)
{
	Pit::Config pc;
	pc.channel = config.pit_channel;
	pc.count = 10000; // Will be set later
	pc.isr = isr;
	pc.is_enable = false;
	return pc;
}

Gpo::Config GetGpoConfig(const PitPwm::Config &config)
{
	Gpo::Config gc;
	gc.pin = config.pin;
	return gc;
}

}

PitPwm::PitPwm(const Config &config)
		: m_pit(GetPitConfig(config,
				  std::bind(&PitPwm::OnTick, this, std::placeholders::_1))),
		  m_pin(GetGpoConfig(config)),
		  m_flag(true),
		  m_precision(config.precision)
{
	Setup(config.period, config.pos_width);
}

void PitPwm::Setup(const uint32_t period, const uint32_t pos_width)
{
	assert(pos_width <= period);

	m_pit.SetEnable(false);

	m_pos_width = pos_width;
	m_neg_width = period - pos_width;
	switch (m_precision)
	{
	default:
	case Pwm::Config::Precision::kUs:
		m_pos_count = ClockUtils::GetBusTickPerUs(m_pos_width);
		m_neg_count = ClockUtils::GetBusTickPerUs(m_neg_width);
		break;

	case Pwm::Config::Precision::KNs:
		m_pos_count = ClockUtils::GetBusTickPerNs(m_pos_width);
		m_neg_count = ClockUtils::GetBusTickPerNs(m_neg_width);
		break;
	}

	if (m_pos_count == 0)
	{
		// 0% duty cycle (always low)
		m_pin.Set(false);
		m_flag = false;
	}
	else if (m_neg_count == 0)
	{
		// 100% duty cycle (always high)
		m_pin.Set(true);
		m_flag = true;
	}
	else
	{
		// Anything in between
		m_pit.SetCount(m_flag ? m_neg_count : m_pos_count);
		m_pit.SetEnable(true);
	}
}

void PitPwm::SetPeriod(const uint32_t period, const uint32_t pos_width)
{
	Setup(period, pos_width);
}

void PitPwm::SetPosWidth(const uint32_t pos_width)
{
	const uint32_t period = m_pos_width + m_neg_width;
	Setup(period, pos_width);
}

void PitPwm::OnTick(Pit*)
{
	if (m_flag)
	{
		m_pin.Set(false);
		m_pit.SetCount(m_neg_count);
		m_flag = false;
	}
	else
	{
		m_pin.Set(true);
		m_pit.SetCount(m_pos_count);
		m_flag = true;
	}
}

}
}
