/*
 * pit_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(pit)

#include "libsc/timer.h"

namespace libsc
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
	void OnTick(LIBBASE_MODULE(Pit)*);

	LIBBASE_MODULE(Pit) m_pit;
	volatile TimerInt m_ms;
};

}
