/*
 * led.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_LED_H_
#define LIBSC_K60_LED_H_

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Led
{
public:
	explicit Led(const uint8_t id);

	void SetEnable(const bool flag);
	void Switch();

private:
	libbase::k60::Gpo m_pin;
};

}
}

#endif /* LIBSC_K60_LED_H_ */