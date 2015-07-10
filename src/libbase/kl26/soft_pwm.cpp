/*
 * soft_pwm.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pit.h"
#include "libbase/kl26/soft_pwm.h"

namespace libbase
{
namespace kl26
{

namespace
{

Pit::Config GetPitConfig(const SoftPwm::Config &config,
		const Pit::OnPitTriggerListener &isr)
{
	Pit::Config pc;
	pc.channel = config.pit_channel;
	pc.count = 10000; // Will be set later
	pc.isr = isr;
	pc.is_enable = false;
	return pc;
}

Gpo::Config GetGpoConfig(const SoftPwm::Config &config)
{
	Gpo::Config gc;
	gc.pin = config.pin;
	return gc;
}

}

SoftPwm::SoftPwm(const Config &config)
		: m_precision(config.precision),
		  m_pit(GetPitConfig(config, std::bind(&SoftPwm::OnTick, this,
				  std::placeholders::_1))),
		  m_pin(GetGpoConfig(config)),
		  m_flag(true),
		  m_is_init(true)
{
	Setup(config.period, config.pos_width);
}

SoftPwm::SoftPwm(SoftPwm &&rhs)
		: SoftPwm(nullptr)
{
	*this = std::move(rhs);
}

SoftPwm::SoftPwm(nullptr_t)
		: m_precision(Pwm::Config::Precision::kUs),
		  m_pit(nullptr),
		  m_pin(nullptr),
		  m_flag(true),
		  m_pos_width(0),
		  m_pos_count(0),
		  m_neg_width(0),
		  m_neg_count(0),
		  m_is_init(false)
{}

SoftPwm::~SoftPwm()
{
	Uninit();
}

SoftPwm& SoftPwm::operator=(SoftPwm &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_precision = rhs.m_precision;
			m_flag = rhs.m_flag;
			m_pos_width = rhs.m_pos_width;
			m_pos_count = rhs.m_pos_count;
			m_neg_width = rhs.m_neg_width;
			m_neg_count = rhs.m_neg_count;

			m_pin = std::move(rhs.m_pin);
			m_pit = std::move(rhs.m_pit);

			m_is_init = true;
		}
	}
	return *this;
}

void SoftPwm::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		m_pit = Pit(nullptr);
	}
}

void SoftPwm::Setup(const uint32_t period, const uint32_t pos_width)
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

	case Pwm::Config::Precision::kNs:
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

void SoftPwm::SetPeriod(const uint32_t period, const uint32_t pos_width)
{
	Setup(period, pos_width);
}

void SoftPwm::SetPosWidth(const uint32_t pos_width)
{
	const uint32_t period = m_pos_width + m_neg_width;
	Setup(period, pos_width);
}

void SoftPwm::OnTick(Pit*)
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
