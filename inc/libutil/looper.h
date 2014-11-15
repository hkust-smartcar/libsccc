/*
 * looper.h
 * Handle main loop and periodically invoking callbacks
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <functional>
#include <list>

#include "libsc/k60/timer.h"

namespace libutil
{

class Looper
{
public:
	typedef std::function<void(const libsc::k60::Timer::TimerInt request,
			const libsc::k60::Timer::TimerInt actual)> Callback;

	Looper();
	~Looper();

	void Loop();
	void Break();

	void RunAfter(const libsc::k60::Timer::TimerInt ms, const Callback &c);

private:
	struct RunnerState;

	void Invoke();

	std::list<RunnerState> m_states;
	bool m_is_run;
};

}
