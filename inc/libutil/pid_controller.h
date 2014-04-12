/*
 * pid_controller.h
 * Generic PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_PID_CONTROLLER_H_
#define LIBUTIL_PID_CONTROLLER_H_

#include <cstdint>

#include <type_traits>

#include "libutil/clock.h"

namespace libutil
{

template<typename T, typename U>
class PidController
{
public:
	typedef T InputType;
	typedef typename std::make_signed<T>::type SignedInputType;
	typedef U OutputType;

	PidController(const InputType setpoint, const float kp, const float ki,
			const float kd);

	OutputType Calc(const Clock::ClockInt time, const InputType current_val);
	OutputType Calc(const InputType current_val)
	{
		return Calc(Clock::Time(), current_val);
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

	void Restart()
	{
		m_prev_time = Clock::Time();
	}

	void Print(const char *label);

private:
	inline void PrintError(const int);
	inline void PrintError(const double);

	InputType m_setpoint;
	float m_kp;
	float m_ki;
	float m_kd;

	SignedInputType m_accumulated_error;
	SignedInputType m_prev_error;
	Clock::ClockInt m_prev_time;
};

}

#endif /* LIBUTIL_PID_CONTROLLER_H_ */
