/*
 * pwm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class Pwm
{
public:
	struct Config
	{
		enum struct Precision
		{
			kUs,
			kNs,
		};

		Pin::Name pin;
		uint32_t period;
		uint32_t pos_width;
		// Should only select NS when period < 1000 us
		Precision precision = Precision::kUs;
	};

	virtual ~Pwm()
	{}

	virtual operator bool() const = 0;

	virtual void SetPeriod(const uint32_t period, const uint32_t pos_width) = 0;
	virtual void SetPosWidth(const uint32_t pos_width) = 0;
};

}
}
