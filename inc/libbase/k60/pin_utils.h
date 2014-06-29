/*
 * pin_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_PIN_UTILS_H_
#define LIBBASE_K60_PIN_UTILS_H_

#include <cstdint>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class PinUtils
{
public:
	static inline Uint GetPort(const PinConfig::Name pin)
	{
		return pin / PinConfig::PORT_PIN_COUNT;
	}

	static inline Uint GetPinNumber(const PinConfig::Name pin)
	{
		return pin % PinConfig::PORT_PIN_COUNT;
	}

	static inline PinConfig::Name GetPin(const Uint port, const Uint pin)
	{
		return (PinConfig::Name)(port * PinConfig::PORT_PIN_COUNT + pin);
	}
};

}
}

#endif /* LIBBASE_K60_PIN_UTILS_H_ */
