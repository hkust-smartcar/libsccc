/*
 * incremental_pid_controller.h
 * Generic incremental PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libsc/k60/timer.h"
#include "libutil/pid_controller.h"

namespace libutil
{

template<typename InT_, typename OutT_>
class IncrementalPidController : public PidController<InT_, OutT_>
{
public:
	typedef InT_ InT;
	typedef OutT_ OutT;

	IncrementalPidController(const InT setpoint, const float kp, const float ki,
			const float kd);

	void Reset()
	{
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
	InT m_prev_error[2];
	OutT m_prev_output;
	libsc::k60::Timer::TimerInt m_prev_time;
};

}

#include "incremental_pid_controller.tcc"
