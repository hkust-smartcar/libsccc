/*
 * timer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include <functional>

#include "libbase/helper.h"
#include LIBBASE_H(clock_utils)
#include LIBBASE_H(pit)

#include "libsc/pit_timer.h"

using namespace LIBBASE_NS;

namespace libsc
{

namespace
{

Pit::Config GetPitConfig(const uint8_t pit_channel,
		const Pit::OnPitTriggerListener &isr)
{
	Pit::Config config;
	config.channel = pit_channel;
	config.count = ClockUtils::GetBusTickPerMs();
	config.isr = isr;
	return config;
}

}

PitTimer::PitTimer(const uint8_t pit_channel)
		: m_pit(GetPitConfig(pit_channel, std::bind(&PitTimer::OnTick, this,
				std::placeholders::_1))),
		  m_ms(0)
{}

void PitTimer::OnTick(Pit*)
{
	++m_ms;
}

}
