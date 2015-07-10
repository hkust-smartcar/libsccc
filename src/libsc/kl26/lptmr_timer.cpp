/*
 * lptmr_timer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include <functional>

#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/lptmr.h"

#include "libsc/kl26/lptmr_timer.h"

using namespace libbase::kl26;

namespace libsc
{
namespace kl26
{

namespace
{

Lptmr::Config GetLptmrConfig(const Lptmr::OnLptmrTriggerListener &isr)
{
	Lptmr::Config product;
	product.count = UINT16_MAX;
	product.isr = isr;
	return product;
}

}

LptmrTimer::LptmrTimer()
		: m_lptmr(GetLptmrConfig(std::bind(&LptmrTimer::OnTick, this,
				std::placeholders::_1))),
		  m_ms(0)
{}

Timer::TimerInt LptmrTimer::Time()
{
	const uint16_t curr_counter = m_lptmr.GetCounter();
	if (curr_counter != m_prev_counter)
	{
		m_ms += (uint16_t)(curr_counter - m_prev_counter);
		m_prev_counter = curr_counter;
	}
	return m_ms;
}

void LptmrTimer::OnTick(Lptmr*)
{
	Time();
}

}
}
