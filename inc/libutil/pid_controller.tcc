/*
 * pid_controller.tcc
 * Generic PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>
#include <cstdio>

#include <log.h>

#include "libsc/k60/system_timer.h"
#include "libsc/k60/timer.h"
#include "libutil/misc.h"
#include "libutil/pid_controller.h"

namespace libutil
{

template<typename T, typename U>
PidController<T, U>::PidController(const InputType setpoint, const float kp,
		const float ki, const float kd)
		: m_setpoint(setpoint), m_kp(kp), m_ki(ki), m_kd(kd), m_i_limit(0.0f),
		  m_accumulated_error(0), m_prev_error(0),
		  m_prev_time(libsc::k60::SystemTimer::Time())
{}

template<typename T, typename U>
inline void PidController<T, U>::PrintSetpoint(const int)
{
	LOG_D("SP: %d", m_setpoint);
}

template<typename T, typename U>
inline void PidController<T, U>::PrintSetpoint(const float)
{
	LOG_D("SP: %f", m_setpoint);
}

template<typename T, typename U>
inline void PidController<T, U>::PrintError(const int32_t error)
{
	LOG_D("Error: prev(%ld) now(%ld) accum(%ld)", m_prev_error, error,
			m_accumulated_error);
}

template<typename T, typename U>
inline void PidController<T, U>::PrintError(const int16_t error)
{
	LOG_D("Error: prev(%d) now(%d) accum(%d)", m_prev_error, error,
			m_accumulated_error);
}

template<typename T, typename U>
inline void PidController<T, U>::PrintError(const float error)
{
	LOG_D("Error: prev(%f) now(%f) accum(%f)", m_prev_error, error,
			m_accumulated_error);
}

template<typename T, typename U>
typename PidController<T, U>::OutputType PidController<T, U>::Calc(
		const libsc::k60::Timer::TimerInt time, const InputType current_val)
{
	const libsc::k60::Timer::TimerInt time_diff =
			libsc::k60::Timer::TimeDiff(time, m_prev_time);
	const InputType error = m_setpoint - current_val;

	m_p = m_kp * error;
	m_accumulated_error += error;
	m_i = m_ki * m_accumulated_error;
	if (m_i_limit > 0.0f)
	{
		const float clamp_i = libutil::Clamp<float>(-m_i_limit, m_i, m_i_limit);
#ifdef LIBUTIL_DEBUG_PID_CONTROLLER
		LOG_D("I clamped: orig(%f), now(%f)", m_i, clamp_i);
#endif
		m_i = clamp_i;
	}
	const float slope = (error - m_prev_error) * 1000 / (float)time_diff;
	m_d = m_kd * slope;

#ifdef LIBUTIL_DEBUG_PID_CONTROLLER
	PrintError(error);
#endif
	m_prev_error = error;
	m_prev_time = time;
#ifdef LIBUTIL_DEBUG_PID_CONTROLLER
	LOG_D("P(%f) + I(%f) - D(%f) = %f", m_p, m_i, m_d, m_p + m_i + m_d);
#endif
	return m_p + m_i + m_d;
}

template<typename T, typename U>
void PidController<T, U>::Print(const char *label)
{
	if (label)
	{
		LOG_I("=== PID State for %s ===", label);
	}
	else
	{
		LOG_I("=== PID State ===");
	}
	LOG_I("KP: %f KI: %f KD: %f", m_kp, m_ki, m_kd);
}

}
