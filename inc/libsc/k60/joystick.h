/*
 * joystick.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
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

	struct Config
	{
		uint8_t id;
		bool is_active_low;
	};

	explicit Joystick(const Config &config);

	State GetState() const;

private:
	libbase::k60::Gpi m_pins[5];
	bool m_is_active_low;
};

}
}
