/*
 * button.h
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

class Button
{
public:
	struct Config
	{
		uint8_t id;
		bool is_active_low;
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
};

}
}
