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
	typedef std::function<void(const uint8_t id)> Listener;

	enum struct State
	{
		kUp = 0,
		kDown,
		kLeft,
		kRight,
		kSelect,

		kIdle
	};

	struct Config
	{
		enum struct Trigger
		{
			kDown,
			kUp,
			kBoth,
		};

		uint8_t id;
		bool is_active_low;
		Listener listeners[5];
		/**
		 * When to trigger the listener, ignored if corresponding listener is
		 * not set in Config::listeners. The sequence of the array follows the
		 * State enum
		 */
		Trigger listener_triggers[5];
	};

	explicit Joystick(const Config &config);

	State GetState() const;

private:
	libbase::k60::Gpi m_pins[5];
	bool m_is_active_low;
	Listener m_isrs[5];
};

}
}
