/*
 * misc_utils_c.h
 * Subset of misc utils that could be used with plain C
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/misc_utils_c.h"

#define __ISR __attribute__((__interrupt__))
#define __RAMFUNC __attribute__((__long_call__, __section__(".m_data_20000000")))
#ifdef DEBUG
#define __BREAKPOINT() asm("BKPT 255")

#else
#define __BREAKPOINT()

#endif
#define __HARDFAULT() *((char*)0) = 42
