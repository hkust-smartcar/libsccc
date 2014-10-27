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
#include "libutil/misc.h"
#include "libutil/pid_controller.h"

namespace libutil
{

template<typename T, typename U>
IncrementalPidController<T, U>::IncrementalPidController(
		const InputType setpoint, const float kp, const float ki, const float kd)
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

template<typename T, typename U>
typename IncrementalPidController<T, U>::OutputType IncrementalPidController<T, U>::Calc(
		const InputType current_val)
{
	const InputType error = m_setpoint - current_val;
	m_p = m_kp * (error - m_prev_error[0]);
	m_i = m_ki * error;
	m_d = m_kd * (error - 2 * m_prev_error[0] + m_prev_error[1]);

	std::swap(m_prev_error[0], m_prev_error[1]);
	m_prev_error[0] = error;

	m_prev_output += m_p + m_i + m_d;
	m_prev_output = libutil::Clamp<float>(m_min_o, m_prev_output, m_max_o);
	return m_prev_output;
}

}
