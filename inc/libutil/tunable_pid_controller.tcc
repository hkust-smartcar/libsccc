/*
 * tunable_pid_controller.tcc
 * Generic tunable PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libutil/clock.h"
#include "libutil/pid_controller.h"
#include "libutil/pid_controller.tcc"
#include "libutil/string.h"
#include "libutil/tunable_int_manager.h"

#include "libutil/tunable_pid_controller.h"

namespace libutil
{

template<typename T, typename U>
template<uint8_t size>
TunablePidController<T, U>::TunablePidController(const InputType setpoint,
		const float kp, const float ki, const float kd, const char *prefix,
		TunableIntManager<size> *manager)
		: PidController(setpoint, kp, ki, kd),
		  m_prefix(prefix)
{
	m_kp = manager->Register(String::Format("%s_kp", m_prefix).c_str(), "float",
			TunableInt::AsUnsigned(kp));
	m_ki = manager->Register(String::Format("%s_ki", m_prefix).c_str(), "float",
			TunableInt::AsUnsigned(ki));
	m_kd = manager->Register(String::Format("%s_kd", m_prefix).c_str(), "float",
			TunableInt::AsUnsigned(kd));
}

template<typename T, typename U>
TunablePidController<T, U>::OutputType TunablePidController<T, U>::Calc(
		const Clock::ClockInt time, const InputType current_val)
{
	SetKp(TunableInt::AsFloat(m_kp->GetValue()));
	SetKi(TunableInt::AsFloat(m_ki->GetValue()));
	SetKd(TunableInt::AsFloat(m_kd->GetValue()));
	return PidController<T, U>::Calc(time, current_val);
}

}
