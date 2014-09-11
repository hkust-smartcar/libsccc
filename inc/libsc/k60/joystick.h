/*
 * joystick.h
 * Joystick
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Joystick
{
public:
	enum struct State
	{
		IDLE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SELECT,
	};

	explicit Joystick(const uint8_t id);

	State GetState() const;

private:
	libbase::k60::Gpi m_pins[5];
};

}
}
