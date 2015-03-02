/*
 * led.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(gpio)

namespace libsc
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
	LIBBASE_MODULE(Gpo) m_pin;
};

}

