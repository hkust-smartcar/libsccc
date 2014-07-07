/*
 * timer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <hw_common.h>

#include <cstdint>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/pit.h"
#include "libsc/k60/timer.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
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

Timer::Timer(const uint8_t pit_channel)
		: m_pit(GetPitConfig(pit_channel, std::bind(&Timer::OnTick, this,
				std::placeholders::_1))), m_ms(0)
{}

Timer::~Timer()
{}

void Timer::OnTick(libbase::k60::Pit*)
{
	++m_ms;
}

}
}
