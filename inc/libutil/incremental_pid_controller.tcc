/*
 * incremental_pid_controller.tcc
 * Generic incremental PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cfloat>
#include <cstdint>
#include <cstdio>
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
		: m_setpoint(setpoint),
		  m_kp(kp),
		  m_ki(ki),
		  m_kd(kd),

		  m_p(0.0f),
		  m_i(0.0f),
		  m_d(0.0f),

		  m_min_o(FLT_MIN),
		  m_max_o(FLT_MAX),

		  m_prev_error{0, 0},
		  m_prev_output(0.0f)
{}

template<typename InT_, typename OutT_>
typename IncrementalPidController<InT_, OutT_>::OutT
IncrementalPidController<InT_, OutT_>::Calc(const InT current_val)
{
	const InT error = m_setpoint - current_val;
	m_p = m_kp * (error - m_prev_error[0]);
	m_i = m_ki * error;
	m_d = m_kd * (error - 2 * m_prev_error[0] + m_prev_error[1]);

	std::swap(m_prev_error[0], m_prev_error[1]);
	m_prev_error[0] = error;

	m_prev_output += m_p + m_i + m_d;
	m_prev_output = libutil::Clamp<OutT>(m_min_o, m_prev_output, m_max_o);
	return m_prev_output;
}

}
