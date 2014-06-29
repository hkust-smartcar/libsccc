/*
 * misc_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_MISC_UTILS_H_
#define LIBBASE_MISC_UTILS_H_

#define SET_BIT(x, n) ((x) |= 1 << (n))
#define RESET_BIT(x, n) ((x) &= ~(1 << (n)))
#define CLEAR_BIT(x, n) RESET_BIT(x, n)
#define GET_BIT(x, n) (((x) >> (n)) & 1)

typedef unsigned int Uint;
typedef unsigned char Byte;

#endif /* LIBBASE_MISC_UTILS_H_ */
