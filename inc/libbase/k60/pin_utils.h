/*
 * pin_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout.h"

namespace libbase
{
namespace k60
{

class PinUtils
{
public:
	static Uint GetPort(const Pin::Name pin)
	{
		return static_cast<Uint>(pin) / PINOUT::GetPortPinCount();
	}

	static Uint GetPinNumber(const Pin::Name pin)
	{
		return static_cast<Uint>(pin) % PINOUT::GetPortPinCount();
	}

	static Pin::Name GetPin(const Uint port, const Uint pin)
	{
		return static_cast<Pin::Name>(port * PINOUT::GetPortPinCount() + pin);
	}
};

}
}
