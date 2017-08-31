/*
 * infra_red_sensor.h
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

class InfraRedSensor
{
public:
	typedef std::function<void(const uint8_t id)> Listener;

	struct Config
	{
		enum struct Trigger
		{
			kEnter,
			kLeave,
			kBoth,
		};

		uint8_t id;
		bool is_active_low;
		Listener listener;
		/// When to trigger the listener, ignored if Config::listener is not set
		Trigger listener_trigger;
	};

	explicit InfraRedSensor(const Config &config);

	bool IsDetected() const;

private:
	LIBBASE_MODULE(Gpi) m_pin;
	bool m_is_active_low;
};

}
