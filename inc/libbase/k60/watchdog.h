/*
 * watchdog.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

namespace libbase
{
namespace k60
{

class Watchdog
{
public:
	typedef void (*OnWatchdogTimeoutListener)();

	struct Config
	{
		bool is_enable = false;
		/// Set the time out value, [50, 8000]
		uint32_t time_out_ms = 1000;
	};

	Watchdog() = delete;

	/**
	 * Must be called at the very beginning in the startup routine
	 */
	static void Init();

	/**
	 * Refresh the Watchdog to prevent it from resetting the system. Should be
	 * called regularly among the time out value
	 */
	static void Refresh();

	static void SetIsr(OnWatchdogTimeoutListener isr);

	///@{
	/**
	 * Alias of Refresh(), choose the one you like most! :)
	 */
	static void Feed()
	{
		Refresh();
	}
	static void Food()
	{
		Refresh();
	}
	static void Bone()
	{
		Refresh();
	}
	static void Play()
	{
		Refresh();
	}
	static void Hand()
	{
		Refresh();
	}
	static void Wan()
	{
		Refresh();
	}
	/// @}

private:
	/**
	 * Get the Config object used during initialization, could be optionally
	 * implemented by user. The default implementation will simply disable the
	 * watchdog
	 *
	 * @return
	 */
	__attribute__((__weak__))
	static Config GetWatchdogConfig();

	static void InitStctrlReg(const Config &config);
	static void InitTimeOutReg(const Config &config);
	static void InitPrescReg();
};

}
}
