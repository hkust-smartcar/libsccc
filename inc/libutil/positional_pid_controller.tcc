/*
 * positional_pid_controller.tcc
 * Generic poitional PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if MK60DZ10 || MK60D10 || MK60F15
#include "libsc/k60/system.h"

#elif MKL26Z4
#include "libsc/kl26/system.h"

#endif

#include "libsc/timer.h"
#include "libutil/misc.h"
#include "libutil/positional_pid_controller.h"

namespace libutil
{

template<typename InT_, typename OutT_>
PositionalPidController<InT_, OutT_>::PositionalPidController(const InT setpoint,
		const float kp, const float ki, const float kd)
		: PidController<InT_, OutT_>(setpoint, kp, ki, kd),
		  m_i_limit(0.0f),

		  m_accumulated_error(0.0f),
		  m_prev_error(0),
#if MK60DZ10 || MK60D10 || MK60F15
		  m_prev_time(libsc::k60::System::Time())
#elif MKL26Z4
		  m_prev_time(libsc::kl26::System::Time())
#endif
{}

template<typename InT_, typename OutT_>
void PositionalPidController<InT_, OutT_>::OnCalc(const InT error)
{
	using namespace libsc;
#if MK60DZ10 || MK60D10 || MK60F15
	using namespace libsc::k60;
#elif MKL26Z4
	using namespace libsc::kl26;
#endif

	const Timer::TimerInt time = System::Time();
	const float time_diff = Timer::TimeDiff(System::Time(), m_prev_time)
			/ 1000.0f;

	const float p = this->GetKp() * error;
	m_accumulated_error += error * time_diff;
	float i = this->GetKi() * m_accumulated_error;
	if (m_i_limit > 0.0f)
	{
		i = Clamp<float>(-m_i_limit, i, m_i_limit);
	}
	const float slope = (error - m_prev_error) / time_diff;
	const float d = this->GetKd() * slope;

	m_prev_error = error;
	m_prev_time = time;
	this->UpdatePid(p, i, d);
}

template<typename InT_, typename OutT_>
OutT_ PositionalPidController<InT_, OutT_>::GetControlOut()
{
	return this->GetP() + this->GetI() + this->GetD();
}

}
