/*
 * pit_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/pit.h"

#include "libsc/k60/timer.h"

namespace libsc
{
namespace k60
{

class PitTimer : public Timer
{
public:
	explicit PitTimer(const uint8_t channel);

	TimerInt Time() override
	{
		return m_ms;
	}

private:
	void OnTick(libbase::k60::Pit*);

	libbase::k60::Pit m_pit;
	volatile TimerInt m_ms;
};

}
}
