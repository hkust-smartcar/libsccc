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

namespace libutil
{

template<typename InT_, typename OutT_>
class IncrementalPidController
{
public:
	typedef typename std::enable_if<std::is_signed<InT_>::value>::type TMustBeSigned;
	typedef InT_ InT;
	typedef OutT_ OutT;

	IncrementalPidController(const InT setpoint, const float kp, const float ki,
			const float kd);

	OutT Calc(const InT current_val);
	OutT Calc(const libsc::k60::Timer::TimerInt time, const InT current_val)
	{
		return Calc(current_val);
	}

	void SetSetpoint(const InT setpoint)
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

	void SetOutputBound(const OutT min, OutT max)
	{
		m_min_o = min;
		m_max_o = max;
	}

	void Reset()
	{}

	void ResetTime()
	{}

	InT GetSetpoint() const
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
	InT m_setpoint;
	float m_kp;
	float m_ki;
	float m_kd;

	float m_p;
	float m_i;
	float m_d;

	OutT m_min_o;
	OutT m_max_o;

	InT m_prev_error[2];
	OutT m_prev_output;
};

}

#include "incremental_pid_controller.tcc"
