/*
 * tunable_pid_controller.tcc
 * Generic tunable PID controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libutil/clock.h"
#include "libutil/string.h"
#include "libutil/tunable_int_manager.h"

#include "libutil/tunable_pid_controller.h"

namespace libutil
{

template<uint8_t size>
TunablePidController::TunablePidController(const uint16_t setpoint,
		const float kp, const float ki, const float kd, const char *prefix,
		TunableIntManager<size> *manager)
		: m_prefix(prefix)
{
	m_kp = manager->Register(String::Format("%s_kp", m_prefix).c_str());
	m_ki = manager->Register(String::Format("%s_ki", m_prefix).c_str());
	m_kd = manager->Register(String::Format("%s_kd", m_prefix).c_str());
}

}
