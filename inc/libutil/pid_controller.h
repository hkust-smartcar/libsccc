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
	typedef typename std::enable_if<std::is_signed<T>::value>::type TMustBeSigned;
	typedef T InputType;
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
		m_accumulated_error = 0;
		m_prev_time = Clock::Time();
	}

	void Print(const char *label);

private:
	inline void PrintSetpoint(const int);
	inline void PrintSetpoint(const float);
	inline void PrintError(const int32_t error);
	inline void PrintError(const int16_t error);
	inline void PrintError(const float error);

	InputType m_setpoint;
	float m_kp;
	float m_ki;
	float m_kd;

	InputType m_accumulated_error;
	InputType m_prev_error;
	Clock::ClockInt m_prev_time;
};

}

#endif /* LIBUTIL_PID_CONTROLLER_H_ */
