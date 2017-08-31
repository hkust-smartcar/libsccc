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

#include "libsc/timer.h"

namespace libutil
{

/**
 * Manage the main loop of a program, and invoke registered functions at the
 * time specified
 *
 * Example:
 * @snippet test/src/kl26/looper_test.cpp code
 */
class Looper
{
public:
	typedef std::function<void(const libsc::Timer::TimerInt request,
			const libsc::Timer::TimerInt actual)> Callback;

	enum struct RepeatMode
	{
		/// Not repeating
		kOnce = 0,
		/// Reduce the time between two calls if the first is delayed
		kPrecise,
		/// Always wait for at least the specified period
		kLoose,
	};

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

	void RunAfter(const libsc::Timer::TimerInt ms, const Callback &c)
	{
		Repeat(ms, c, RepeatMode::kOnce);
	}
	/**
	 * Repeatly call @a c per @a ms ms
	 *
	 * @param ms
	 * @param c
	 * @param mode
	 * @see Looper::RepeatMode
	 */
	void Repeat(const libsc::Timer::TimerInt ms, const Callback &c,
			const RepeatMode mode);

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
	libsc::Timer::TimerInt m_prev;
	bool m_is_run;
};

}
