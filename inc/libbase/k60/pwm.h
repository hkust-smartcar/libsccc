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
		PinConfig::Name pin;
		uint32_t period_us;
		uint32_t high_time_us;
	};

	virtual ~Pwm()
	{}

	virtual void SetPeriodUs(const uint32_t period_us,
			const uint32_t high_time_us) = 0;
	virtual void SetHighTimeUs(const uint32_t high_time_us) = 0;
};

}
}

#endif /* LIBBASE_K60_PWM_H_ */
