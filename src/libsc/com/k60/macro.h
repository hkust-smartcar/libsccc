/*
 * macro.h
 * Useful macros
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_MACRO_H_
#define LIBSC_MACRO_H_

#include "libutil/misc.h"

#define LIBSC_PIN_OUT(pin) UTIL_JOIN(pin, _OUT)
#define LIBSC_PIN_IN(pin) UTIL_JOIN(pin, _IN)
#define LIBSC_PIN_DDR(pin) UTIL_JOIN(pin, _DDR)

#endif /* LIBSC_MACRO_H_ */
