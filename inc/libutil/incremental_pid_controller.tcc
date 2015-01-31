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

#include "libutil/incremental_pid_controller.h"
#include "libutil/misc.h"

namespace libutil
{

template<typename InT_, typename OutT_>
IncrementalPidController<InT_, OutT_>::IncrementalPidController(
		const InT setpoint, const float kp, const float ki, const float kd)
		: PidController<InT_, OutT_>(setpoint, kp, ki, kd),
		  m_prev_error{0, 0},
		  m_prev_output(0)
{}

template<typename InT_, typename OutT_>
void IncrementalPidController<InT_, OutT_>::OnCalc(const InT error)
{
	const float p = this->GetKp() * (error - m_prev_error[0]);
	const float i = this->GetKi() * error;
	const float d = this->GetKd() * (error - 2 * m_prev_error[0]
			+ m_prev_error[1]);

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
