/*
 * pid_controller.tcc
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 Ming Tsang
 * Refer to LICENSE for details
 */

#pragma once

#include <cfloat>
#include <limits>

#include "libutil/pid_controller.h"
#include "libutil/misc.h"

namespace libutil
{

template<typename InT_, typename OutT_>
PidController<InT_, OutT_>::PidController(const InT setpoint, const float kp,
		const float ki, const float kd)
		: m_setpoint(setpoint),
		  m_kp(kp),
		  m_ki(ki),
		  m_kd(kd),

		  m_p(0.0f),
		  m_i(0.0f),
		  m_d(0.0f),

		  m_min_o(std::numeric_limits<OutT>::min()),
		  m_max_o(std::numeric_limits<OutT>::max())
{}

template<typename InT_, typename OutT_>
OutT_ PidController<InT_, OutT_>::Calc(const InT current_val)
{
	const InT error = m_setpoint - current_val;
	OnCalc(error);
	m_prev_out = libutil::Clamp<OutT>(m_min_o, GetControlOut(), m_max_o);
	return m_prev_out;
}

template<typename InT_, typename OutT_>
void PidController<InT_, OutT_>::UpdatePid(const float p, const float i,
		const float d)
{
	m_p = p;
	m_i = i;
	m_d = d;
}

}
