/*
 * timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_TIMER_H_
#define LIBSC_K60_TIMER_H_

#include <cstdint>

#include "libbase/k60/pit.h"

namespace libsc
{
namespace k60
{

class Timer
{
public:
	typedef uint32_t TimerInt;

	explicit Timer(const uint8_t pit_channel);
	~Timer();

	/**
	 * Return the time elapsed, in ms
	 *
	 * @return
	 */
	TimerInt Time()
	{
		return m_ms;
	}

	/**
	 * Return how @a a is apart from @a b
	 *
	 * @param a Some time in the future, in terms of @a b
	 * @param b Some time in the past, in terms of @a a
	 * @return
	 */
	static TimerInt TimeDiff(const TimerInt a, const TimerInt b)
	{
		return (TimerInt)(a - b);
	}

private:
	void OnTick(libbase::k60::Pit*);

	libbase::k60::Pit m_pit;
	volatile TimerInt m_ms;
};

}
}

#endif /* LIBSC_K60_TIMER_H_ */
