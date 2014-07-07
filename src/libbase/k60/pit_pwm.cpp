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
		  m_flag(true)
{
	Setup(config.period_us, config.high_time_us);
}

void PitPwm::Setup(const uint32_t period_us, const uint32_t high_time_us)
{
	assert(high_time_us <= period_us);

	m_pit.SetEnable(false);

	m_high_time = high_time_us;
	m_high_count = ClockUtils::GetBusTickPerUs() * m_high_time;
	m_low_time = period_us - high_time_us;
	m_low_count = ClockUtils::GetBusTickPerUs() * m_low_time;

	if (m_high_count == 0)
	{
		// 0% duty cycle (always low)
		m_pin.Set(false);
		m_flag = false;
	}
	else if (m_low_count == 0)
	{
		// 100% duty cycle (always high)
		m_pin.Set(true);
		m_flag = true;
	}
	else
	{
		// Anything in between
		m_pit.SetCount(m_flag ? m_low_count : m_high_count);
		m_pit.SetEnable(true);
	}
}

void PitPwm::SetPeriodUs(const uint32_t period_us, const uint32_t high_time_us)
{
	Setup(period_us, high_time_us);
}

void PitPwm::SetHighTimeUs(const uint32_t high_time_us)
{
	const uint32_t period = m_high_time + m_low_time;
	assert(high_time_us <= period);
	Setup(period, high_time_us);
}

void PitPwm::OnTick(Pit*)
{
	if (m_flag)
	{
		m_pin.Set(false);
		m_pit.SetCount(m_low_count);
		m_flag = false;
	}
	else
	{
		m_pin.Set(true);
		m_pit.SetCount(m_high_count);
		m_flag = true;
	}
}

}
}
