/*
 * pid_controller.h
 * Generic poitional PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <type_traits>

#include "libsc/k60/system.h"
#include "libsc/k60/timer.h"

namespace libutil
{

template<typename T, typename U>
class PidController
{
public:
	typedef typename std::enable_if<std::is_signed<T>::value>::type TMustBeSigned;
	typedef T InputType;
	typedef U OutputType;

	PidController(const InputType setpoint, const float kp, const float ki,
			const float kd);

	OutputType Calc(const libsc::k60::Timer::TimerInt time,
			const InputType current_val);
	OutputType Calc(const InputType current_val)
	{
		return Calc(libsc::k60::System::Time(), current_val);
	}

	void SetSetpoint(const InputType setpoint)
	{
		m_setpoint = setpoint;
	}

	void SetKp(const float kp)
	{
		m_kp = kp;
	}

	void SetKi(const float ki)
	{
		m_ki = ki;
	}

	void SetKd(const float kd)
	{
		m_kd = kd;
	}

	/**
	 * Set the upper bound of i, <= 0 means unlimited i
	 *
	 * @param value
	 */
	void SetILimit(const float value)
	{
		m_i_limit = value;
	}

	void SetOutputBound(const OutputType min, OutputType max)
	{
		m_min_o = min;
		m_max_o = max;
	}

	void Reset()
	{
		m_accumulated_error = 0;
		ResetTime();
	}

	void ResetTime()
	{
		m_prev_time = libsc::k60::System::Time();
	}

	InputType GetSetpoint() const
	{
		return m_setpoint;
	}

	float GetKp() const
	{
		return m_kp;
	}

	float GetKi() const
	{
		return m_ki;
	}

	float GetKd() const
	{
		return m_kd;
	}

	float GetP() const
	{
		return m_p;
	}

	float GetI() const
	{
		return m_i;
	}

	float GetD() const
	{
		return m_d;
	}

private:
	InputType m_setpoint;
	float m_kp;
	float m_ki;
	float m_kd;

	float m_p;
	float m_i;
	float m_d;

	OutputType m_min_o;
	OutputType m_max_o;
	float m_i_limit;

	float m_accumulated_error;
	InputType m_prev_error;
	libsc::k60::Timer::TimerInt m_prev_time;
};

}

#include "pid_controller.tcc"
