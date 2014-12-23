/*
 * switch.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Switch
{
public:
	struct Config
	{
		uint8_t id;
		// on == active
		bool is_active_low;
	};

	explicit Switch(const Config &config);

	bool IsOn() const;
	bool IsOff() const
	{
		return !IsOn();
	}

private:
	libbase::k60::Gpi m_pin;
	bool m_is_active_low;
};

}
}
