/*
 * macro.h
 * Useful macros
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_MACRO_H_
#define LIBSC_MACRO_H_

#define LIBSC_JOIN(x, y) x ## y
#define LIBSC_PIN_OUT(pin) LIBSC_JOIN(pin, _OUT)

#endif /* LIBSC_MACRO_H_ */
