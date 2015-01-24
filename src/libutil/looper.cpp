/*
 * looper.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>
#include <list>

#include "libsc/k60/system.h"
#include "libsc/k60/timer.h"
#include "libutil/looper.h"

using namespace libsc::k60;
using namespace std;

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
	Timer::TimerInt prev = System::Time();
	while (m_is_run)
	{
		const Timer::TimerInt now = System::Time();
		if (now != prev)
		{
			Invoke();
			prev = now;
		}
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

}
