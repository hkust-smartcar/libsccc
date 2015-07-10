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

/**
 * LED
 *
 * Example:
 * @snippet test/src/k60/led_test.cpp conf
 * @snippet test/src/k60/led_test.cpp code
 */
class Led
{
public:
	struct Config
	{
		uint8_t id;
		bool is_active_low;
	};

	explicit Led(const Config &config);

	void SetEnable(const bool flag);
	void Switch();

protected:
	LIBBASE_MODULE(Gpo)* GetPin()
	{
		return &m_pin;
	}

private:
	LIBBASE_MODULE(Gpo) m_pin;
	bool m_is_active_low;
};

}

