/*
 * watchdog.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class Watchdog
{
public:
	typedef std::function<void(void)> OnWatchdogTimeoutListener;

	struct Config
	{
		bool is_enable = false;
		/// Set whether to allow updating the Watchdog after the initial write
		bool is_allow_update = true;
		/// Set the time out value, [50, 8000]
		uint16_t time_out_ms = 1000;

		OnWatchdogTimeoutListener isr;
	};

	explicit Watchdog(const Config &config);
	Watchdog(const Watchdog&) = delete;
	~Watchdog();

	Watchdog& operator=(const Watchdog&) = delete;

	/**
	 * Must be called at the very beginning in the startup routine
	 */
	static void StartupInitialize();

	/**
	 * Refresh the Watchdog to prevent it from resetting the system. Should be
	 * called regularly among the time out value
	 */
	void Refresh();

private:
	void InitStctrlReg(const Config &config);
	void InitTimeOutReg(const Config &config);
	void InitPrescReg(const Config &config);

	static __ISR void IrqHandler();

	OnWatchdogTimeoutListener m_isr;
};

}
}
