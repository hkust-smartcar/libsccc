/*
 * pin_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */


#pragma once

#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout.h"

namespace libbase
{
namespace kl26
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

