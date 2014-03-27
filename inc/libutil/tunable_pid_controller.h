/*
 * tunable_pid_controller.h
 * Generic tunable PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_TUNABLE_PID_CONTROLLER_H_
#define LIBUTIL_TUNABLE_PID_CONTROLLER_H_

#include <cstdint>

#include "libutil/clock.h"
#include "libutil/pid_controller.h"
#include "libutil/tunable_int_manager.h"

namespace libutil
{

template<typename T, typename U>
class TunablePidController<T, U> : public PidController<T, U>
{
public:
	template<uint8_t size>
	TunablePidController(const InputType setpoint, const float kp,
			const float ki, const float kd, const char *prefix,
			TunableIntManager<size> *manager);

	OutputType Calc(const Clock::ClockInt time, const InputType current_val);

private:
	const char *m_prefix;
	const TunableInt *m_kp;
	const TunableInt *m_ki;
	const TunableInt *m_kd;
};

}

#endif /* LIBUTIL_TUNABLE_PID_CONTROLLER_H_ */
