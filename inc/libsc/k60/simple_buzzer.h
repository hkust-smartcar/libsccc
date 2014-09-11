/*
 * simple_buzzer.h
 * Simple Buzzer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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
	explicit SimpleBuzzer(const uint8_t id);

	void SetBeep(const bool is_beep);

private:
	libbase::k60::Gpo m_pin;
};

}
}
