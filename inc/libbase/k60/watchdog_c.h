/*
 * watchdog_c.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/misc_utils_c.h"

#ifdef __cplusplus
extern "C" {
#endif

void LibbaseK60WatchdogInit(void);
__ISR void LibbaseK60WatchdogIsr(void);

#ifdef __cplusplus
}
#endif
