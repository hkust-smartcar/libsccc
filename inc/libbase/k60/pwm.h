/*
 * pwm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_PWM_H_
#define LIBBASE_K60_PWM_H_

#include <cstdint>

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
			US,
			NS,
		};

		PinConfig::Name pin;
		uint32_t period;
		uint32_t pos_width;
		// Should only select NS when period < 1000 us
		Precision precision = Precision::US;
	};

	virtual ~Pwm()
	{}

	virtual void SetPeriod(const uint32_t period, const uint32_t pos_width) = 0;
	virtual void SetPosWidth(const uint32_t pos_width) = 0;
};

}
}

#endif /* LIBBASE_K60_PWM_H_ */
