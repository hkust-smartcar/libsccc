/*
 * light_sensor.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <functional>

#include "libbase/helper.h"
#include LIBBASE_H(gpio)

namespace libsc
{

class LightSensor
{
public:
	typedef std::function<void(const uint8_t id)> Listener;

	struct Config
	{
		enum struct Trigger
		{
			kBright,
			kDark,
			kBoth,
		};

		uint8_t id;
		bool is_active_low;
		Listener listener;
		/// When to trigger the listener, ignored if Config::listener is not set
		Trigger listener_trigger;
	};

	explicit LightSensor(const Config &config);

	bool IsBright() const;
	bool IsDark() const
	{
		return !IsBright();
	}

private:
	LIBBASE_MODULE(Gpi) m_pin;
	bool m_is_active_low;
};

}
