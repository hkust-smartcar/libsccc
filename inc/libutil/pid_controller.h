/*
 * pid_controller.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 Ming Tsang
 * Refer to LICENSE for details
 */

#pragma once

#include <type_traits>

namespace libutil
{

template<typename InT_, typename OutT_>
class PidController
{
public:
	typedef typename std::enable_if<std::is_signed<InT_>::value>::type
			InTMustBeSigned;
	typedef InT_ InT;
	typedef OutT_ OutT;

	PidController(const InT setpoint, const float kp, const float ki,
			const float kd);

	OutT Calc(const InT current_val);

	void SetSetpoint(const InT setpoint)
	{
		m_setpoint = setpoint;
	}

	InT GetSetpoint() const
	{
		return m_setpoint;
	}

	void SetKp(const float kp)
	{
		m_kp = kp;
	}

	float GetKp() const
	{
		return m_kp;
	}

	void SetKi(const float ki)
	{
		m_ki = ki;
	}

	float GetKi() const
	{
		return m_ki;
	}

	void SetKd(const float kd)
	{
		m_kd = kd;
	}

	float GetKd() const
	{
		return m_kd;
	}

	void SetOutputBound(const OutT min, OutT max)
	{
		m_min_o = min;
		m_max_o = max;
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

	OutT GetPrevOut() const
	{
		return m_prev_out;
	}

protected:
	~PidController()
	{}

	virtual void OnCalc(const InT error) = 0;
	virtual OutT GetControlOut() = 0;
	void UpdatePid(const float p, const float i, const float d);

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
	OutT m_prev_out;
};

}

#include "pid_controller.tcc"
