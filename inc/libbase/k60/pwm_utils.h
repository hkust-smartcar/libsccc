/*
 * pwm_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_PWM_UTILS_H_
#define LIBBASE_K60_PWM_UTILS_H_

namespace libbase
{
namespace k60
{

class PwmUtils
{
public:
	static uint32_t GetPeriodUs(const uint32_t frequency)
	{
		return 1000000 / frequency;
	}

	static uint32_t GetHighTimeUs(const uint32_t period_us,
			const Uint duty_10000)
	{
		return period_us * duty_10000 / 10000;
	}
};

}
}

#endif /* LIBBASE_K60_PWM_UTILS_H_ */
