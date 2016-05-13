/*
 * joystick.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(gpio)

namespace libsc
{

class Joystick
{
public:

	enum struct State
	{
		kUp = 0,
		kDown,
		kLeft,
		kRight,
		kSelect,

		kIdle
	};

	typedef std::function<void(const uint8_t id, const State which)> Listener;

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

		/**
		 * Choose either five handlers or one dispatcher to handle all events.
		 * Once you set dispatcher, all listeners will be ignored.
		 */
		Listener dispatcher;
		Listener handlers[5];

		/**
		 * When to trigger the listener, ignored if corresponding listener is
		 * not set in Config::listeners. The sequence of the array follows the
		 * State enum
		 */
		Trigger listener_triggers[5];
	};

	explicit Joystick(const Config &config);
	explicit Joystick(nullptr_t);

	State GetState() const;

private:
	LIBBASE_MODULE(Gpi) m_pins[5];
	bool m_is_active_low;
};

}
