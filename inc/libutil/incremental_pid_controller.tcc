/*
 * incremental_pid_controller.tcc
 * Generic incremental PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <algorithm>

#include "libsc/k60/system.h"
#include "libsc/k60/timer.h"
#include "libutil/incremental_pid_controller.h"
#include "libutil/misc.h"

namespace libutil
{

template<typename InT_, typename OutT_>
IncrementalPidController<InT_, OutT_>::IncrementalPidController(
		const InT setpoint, const float kp, const float ki, const float kd)
		: PidController<InT_, OutT_>(setpoint, kp, ki, kd),
		  m_i_limit(0.0f),

		  m_prev_error{0, 0},
		  m_prev_output(0),
		  m_prev_time(libsc::k60::System::Time())
{}

template<typename InT_, typename OutT_>
void IncrementalPidController<InT_, OutT_>::OnCalc(const InT error)
{
	using namespace libsc::k60;

	const Timer::TimerInt time = System::Time();
	const float time_diff = Timer::TimeDiff(System::Time(), m_prev_time)
			/ 1000.0f;

	const float p = this->GetKp() * (error - m_prev_error[0]);
	float i = this->GetKi() * (error + m_prev_error[0]) * time_diff * 0.5f;
	if (m_i_limit > 0.0f)
	{
		i = libutil::Clamp<float>(-m_i_limit, i, m_i_limit);
	}
	const float d = this->GetKd() * (error - 2 * m_prev_error[0]
			+ m_prev_error[1]) / time_diff;

	std::swap(m_prev_error[0], m_prev_error[1]);
	m_prev_error[0] = error;
	this->UpdatePid(p, i, d);
}

template<typename InT_, typename OutT_>
OutT_ IncrementalPidController<InT_, OutT_>::GetControlOut()
{
	return this->GetP() + this->GetI() + this->GetD();
}

}
