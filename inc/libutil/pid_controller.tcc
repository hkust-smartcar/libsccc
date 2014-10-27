/*
 * pid_controller.tcc
 * Generic poitional PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cfloat>
#include <cstdint>
#include <cstdio>

#include "libsc/k60/system.h"
#include "libsc/k60/timer.h"
#include "libutil/misc.h"
#include "libutil/pid_controller.h"

namespace libutil
{

template<typename T, typename U>
PidController<T, U>::PidController(const InputType setpoint, const float kp,
		const float ki, const float kd)
		: m_setpoint(setpoint),
		  m_kp(kp),
		  m_ki(ki),
		  m_kd(kd),

		  m_p(0.0f),
		  m_i(0.0f),
		  m_d(0.0f),

		  m_min_o(FLT_MIN),
		  m_max_o(FLT_MAX),
		  m_i_limit(0.0f),

		  m_accumulated_error(0.0f),
		  m_prev_error(0),
		  m_prev_time(libsc::k60::System::Time())
{}

template<typename T, typename U>
typename PidController<T, U>::OutputType PidController<T, U>::Calc(
		const libsc::k60::Timer::TimerInt time, const InputType current_val)
{
	const float time_diff = libsc::k60::Timer::TimeDiff(time, m_prev_time)
			/ 1000.0f;
	const InputType error = m_setpoint - current_val;

	m_p = m_kp * error;
	m_accumulated_error += error * time_diff;
	m_i = m_ki * m_accumulated_error;
	if (m_i_limit > 0.0f)
	{
		m_i = libutil::Clamp<float>(-m_i_limit, m_i, m_i_limit);
	}
	const float slope = (error - m_prev_error) / time_diff;
	m_d = m_kd * slope;

	m_prev_error = error;
	m_prev_time = time;
	return libutil::Clamp<float>(m_min_o, m_p + m_i + m_d, m_max_o);
}

}
