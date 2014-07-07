/*
 *  * system_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_SYSTEM_DELAY_H_
#define LIBSC_K60_SYSTEM_DELAY_H_

#include <cstdint>

#include "libbase/k60/sys_tick.h"

namespace libsc
{
namespace k60
{

class SystemDelay
{
public:
	static void Init()
	{
		if (!m_delay)
		{
			m_delay = new libbase::k60::SysTickDelay;
		}
	}

	static void DelayUs(const uint16_t us)
	{
		m_delay->DelayUs(us);
	}

	static void DelayMs(const uint16_t ms)
	{
		m_delay->DelayMs(ms);
	}

	static void DelayS(const uint16_t s)
	{
		m_delay->DelayS(s);
	}

private:
	static libbase::k60::SysTickDelay *m_delay;
};

}
}

#endif /* LIBSC_K60_SYSTEM_DELAY_H_ */
