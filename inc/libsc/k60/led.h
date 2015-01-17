/*
 * led.h
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

class Led
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit Led(const Config &config);

	void SetEnable(const bool flag);
	void Switch();

private:
	libbase::k60::Gpo m_pin;
};

}
}

