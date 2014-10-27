/*
 * incremental_pid_controller.h
 * Generic incremental PID controller
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
class IncrementalPidController
{
public:
	typedef typename std::enable_if<std::is_signed<T>::value>::type TMustBeSigned;
	typedef T InputType;
	typedef U OutputType;

	IncrementalPidController(const InputType setpoint, const float kp,
			const float ki, const float kd);

	OutputType Calc(const InputType current_val);
	OutputType Calc(const libsc::k60::Timer::TimerInt time,
			const InputType current_val)
	{
		return Calc(current_val);
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

	void SetILimit(const float)
	{}

	void SetOutputBound(const OutputType min, OutputType max)
	{
		m_min_o = min;
		m_max_o = max;
	}

	void Reset()
	{}

	void ResetTime()
	{}

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

	InputType m_prev_error[2];
	OutputType m_prev_output;
};

}

#include "incremental_pid_controller.tcc"
