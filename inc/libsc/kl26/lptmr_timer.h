/*
 * lptmr_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/kl26/lptmr.h"

#include "libsc/timer.h"

namespace libsc
{
namespace kl26
{

class LptmrTimer : public Timer
{
public:
	LptmrTimer();

	TimerInt Time() override;

private:
	void OnTick(libbase::kl26::Lptmr*);

	libbase::kl26::Lptmr m_lptmr;
	volatile TimerInt m_ms;
	volatile uint16_t m_prev_counter;
};

}
}
