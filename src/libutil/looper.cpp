/*
 * looper.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <functional>
#include <list>

#include "libsc/system.h"
#include "libsc/timer.h"
#include "libutil/looper.h"

using namespace libsc;
using namespace std;

namespace libutil
{

struct Looper::RunnerState
{
	Timer::TimerInt request;
	Timer::TimerInt start;
	Callback callback;
	Looper::RepeatMode mode;
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
		const Timer::TimerInt now = System::Time();
		const Timer::TimerInt duration = Timer::TimeDiff(now, it->start);
		if (duration >= it->request)
		{
			it->callback(it->request, duration);
			switch (it->mode)
			{
			case RepeatMode::kOnce:
				it = m_states.erase(it);
				break;

			case RepeatMode::kPrecise:
				it->start += it->request;
				++it;
				break;

			case RepeatMode::kLoose:
				it->start = now;
				++it;
				break;
			}
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

void Looper::Repeat(const Timer::TimerInt ms, const Callback &c,
		const RepeatMode mode)
{
	RunnerState rs;
	rs.request = ms;
	rs.start = System::Time();
	rs.callback = c;
	rs.mode = mode;
	m_states.push_back(std::move(rs));
}

void Looper::ResetTiming()
{
	m_prev = System::Time();
}

}
