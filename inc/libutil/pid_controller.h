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

#include "libutil/clock.h"

namespace libutil
{

class PidController
{
public:
	PidController(const uint16_t setpoint, const float kp, const float ki,
			const float kd);

	uint16_t Calc(const Clock::ClockInt time, const uint16_t current_val);

	void SetSetpoint(const uint16_t setpoint)
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

private:
	uint16_t m_setpoint;
	float m_kp;
	float m_ki;
	float m_kd;

	int32_t m_accumulated_error;
	int16_t m_prev_error;
	Clock::ClockInt m_prev_time;
};

}

#endif /* LIBUTIL_PID_CONTROLLER_H_ */
