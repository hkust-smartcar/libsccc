/*
 * clock.h
 * Clock util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_CLOCK_H_
#define LIBUTIL_CLOCK_H_

#include <cstdint>

#include "vectors.h"

namespace libutil
{

class Clock
{
public:
	typedef uint16_t ClockInt;

	static void Init();
	static ClockInt Time()
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
	static ClockInt TimeDiff(const ClockInt a, const ClockInt b)
	{
		return (ClockInt)(a - b);
	}

private:
	static __ISR void IrqHandler();

	static volatile ClockInt m_ms;
};

}

#endif /* LIBUTIL_CLOCK_H_ */
