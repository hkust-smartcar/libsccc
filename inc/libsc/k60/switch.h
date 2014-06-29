/*
 * switch.h
 * Switch
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_SWITCH_H_
#define LIBSC_K60_SWITCH_H_

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Switch
{
public:
	explicit Switch(const uint8_t id);

	bool IsOn() const;
	bool IsOff() const
	{
		return !IsOn();
	}

private:
	libbase::k60::Gpi m_pin;
};

}
}

#endif /* LIBSC_K60_SWITCH_H_ */
