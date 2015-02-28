/*
 * watchdog.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

namespace libbase
{
namespace kl26
{

class Watchdog
{
public:
	struct Config
	{
		enum struct Timeout
		{
			kDisable = 0,
			k32Ms,
			k256Ms,
			k1024Ms,
		};

		Timeout timeout = Timeout::kDisable;
	};

	static Watchdog& Get()
	{
		static Watchdog inst;
		return inst;
	}

	/**
	 * Must be called at the very beginning in the startup routine
	 */
	void Init();

	/**
	 * Refresh the Watchdog to prevent it from resetting the system. Should be
	 * called regularly among the time out value
	 */
	static void Refresh();

private:
	Watchdog();

	/**
	 * Get the Config object used during initialization, could be optionally
	 * implemented by user. The default implementation will simply disable the
	 * watchdog
	 *
	 * @return
	 */
	__attribute__((__weak__))
	static Config GetWatchdogConfig();

	void InitCopcReg(const Config &config);
};

}
}
