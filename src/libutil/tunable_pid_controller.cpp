/*
 * tunable_pid_controller.cpp
 * Generic tunable PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libutil/clock.h"
#include "libutil/tunable_int_manager.h"

#include "libutil/tunable_pid_controller.h"

namespace libutil
{

uint16_t TunablePidController::Calc(const Clock::ClockInt time,
		const uint16_t current_val)
{
	SetKp(TunableInt::AsFloat(m_kp->GetValue()));
	SetKi(TunableInt::AsFloat(m_ki->GetValue()));
	SetKd(TunableInt::AsFloat(m_kd->GetValue()));
	return PidController::Calc(time, current_val);
}

}
