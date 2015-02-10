/*
 * button.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <functional>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Button
{
public:
	typedef std::function<void(const uint8_t id)> Listener;

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
		Listener listener;
		/// When to trigger the listener, ignored if @a listener is not set
		Trigger listener_trigger;
	};

	explicit Button(const Config &config);

	bool IsDown() const;
	bool IsUp() const
	{
		return !IsDown();
	}

private:
	libbase::k60::Gpi m_pin;
	bool m_is_active_low;
	Listener m_isr;
};

}
}
