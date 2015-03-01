/*
 * looper.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>
#include <list>

#if MK60DZ10 || MK60D10 || MK60F15
#include "libsc/k60/system.h"

#elif MKL26Z4
#include "libsc/kl26/system.h"

#endif

#include "libsc/timer.h"
#include "libutil/looper.h"

using namespace libsc;
using namespace std;

#if MK60DZ10 || MK60D10 || MK60F15
using namespace libsc::k60;
#elif MKL26Z4
using namespace libsc::kl26;
#endif

namespace libutil
{

struct Looper::RunnerState
{
	Timer::TimerInt request;
	Timer::TimerInt start;
	Callback callback;
};

Looper::Looper()
		: m_is_run(true)
{}

Looper::~Looper()
{}

void Looper::Loop()
{
	ResetTiming();
	while (m_is_run)
	{
		Once();
	}
}

void Looper::Once()
{
	const Timer::TimerInt now = System::Time();
	if (now != m_prev)
	{
		Invoke();
		m_prev = now;
	}
}

void Looper::Invoke()
{
	list<RunnerState>::iterator it = m_states.begin();
	while (it != m_states.end())
	{
		const Timer::TimerInt duration = Timer::TimeDiff(System::Time(),
				it->start);
		if (duration >= it->request)
		{
			it->callback(it->request, duration);
			it = m_states.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Looper::Break()
{
	m_is_run = false;
}

void Looper::RunAfter(const Timer::TimerInt ms, const Callback &c)
{
	RunnerState rs;
	rs.request = ms;
	rs.start = System::Time();
	rs.callback = c;
	m_states.push_back(std::move(rs));
}

void Looper::ResetTiming()
{
	m_prev = System::Time();
}

}
