/*
 * motor.cpp
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libsc/k60/motor.h"
#include "libutil/misc.h"

namespace libsc
{
namespace k60
{

Motor::Motor(const Config &config)
		: m_multiplier(config.multiplier),
		  m_power(0),
		  m_is_clockwise(true)
{}

void Motor::SetPower(const uint16_t power)
{
	const uint16_t real_power = libutil::Clamp<Uint>(0,
			power * m_multiplier / 100, 1000);
	if (m_power == real_power)
	{
		return;
	}

	OnSetPower(real_power);
	m_power = real_power;
}

void Motor::AddPower(const int16_t power)
{
	SetPower(libutil::Clamp<int>(0, m_power + power, 1000));
}

void Motor::SetClockwise(const bool flag)
{
	if (m_is_clockwise == flag)
	{
		return;
	}

	OnSetClockwise(flag);
	m_is_clockwise = flag;
}

}
}
