/*
 * simple_buzzer.h
 * Simple Buzzer
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

class SimpleBuzzer
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit SimpleBuzzer(const Config &config);

	void SetBeep(const bool is_beep);
	bool GetBeep() const;

private:
	libbase::k60::Gpo m_pin;
};

}
}
