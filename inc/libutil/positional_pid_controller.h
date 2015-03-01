/*
 * positional_pid_controller.h
 * Generic poitional PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libsc/k60/system.h"
#include "libsc/timer.h"
#include "libutil/pid_controller.h"

namespace libutil
{

template<typename InT_, typename OutT_>
class PositionalPidController : public PidController<InT_, OutT_>
{
public:
	typedef InT_ InT;
	typedef OutT_ OutT;

	PositionalPidController(const InT setpoint, const float kp, const float ki,
			const float kd);

	/**
	 * Set the upper bound of i, <= 0 means unlimited i
	 *
	 * @param value
	 */
	void SetILimit(const float value)
	{
		m_i_limit = value;
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

protected:
	void OnCalc(const InT error) override;
	OutT GetControlOut() override;

private:
	float m_i_limit;

	float m_accumulated_error;
	InT m_prev_error;
	libsc::Timer::TimerInt m_prev_time;
};

}

#include "positional_pid_controller.tcc"
