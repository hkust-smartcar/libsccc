/*
 * looper.h
 * Handle main loop and periodically invoking callbacks
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
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

	/**
	 * Start the main loop. This method would not return unless Break() is
	 * called
	 */
	void Loop();
	void Break();
	/**
	 * Run the looper once and return immediately. Mainly used when you want
	 * a custom main loop, in that case, you should call ResetTiming() first
	 * before entering such loop
	 */
	void Once();

	void RunAfter(const libsc::k60::Timer::TimerInt ms, const Callback &c);

	/**
	 * Reset the internal time
	 */
	void ResetTiming();

	/**
	 * Return whether Break() has been called
	 *
	 * @return
	 */
	bool IsBreak() const
	{
		return !m_is_run;
	}

private:
	struct RunnerState;

	void Invoke();

	std::list<RunnerState> m_states;
	libsc::k60::Timer::TimerInt m_prev;
	bool m_is_run;
};

}
