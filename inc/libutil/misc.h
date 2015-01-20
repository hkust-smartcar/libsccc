/*
 * misc.h
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#ifndef LIBUTIL_MISC_H_
#define LIBUTIL_MISC_H_

#include <cstdint>

#include <algorithm>

#include "libbase/k60/misc_utils.h"

namespace libsc
{
namespace k60
{

class UartDevice;

}
}

#define SAFE_DELETE(x) do{if (x) {delete x; x = nullptr;}}while(false)

#define UTIL_JOIN(x, y) x ## y

namespace libutil
{

template<typename T>
inline T Clamp(const T &min, const T &x, const T &max)
{
	return std::max(min, std::min(x, max));
}

template<typename T>
inline T ClampVal(const T min, const T x, const T max)
{
	return (x > min) ? ((x > max) ? max : x) : min;
}

inline uint16_t GetRgb565(const uint8_t r, const uint8_t g, const uint8_t b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void InitDefaultFwriteHandler(libsc::k60::UartDevice *uart);
void UninitDefaultFwriteHandler();

template<typename T>
inline T EnumAdvance(const T e, const int n)
{
	return static_cast<T>(static_cast<int>(e) + n);
}

template<typename T, typename U>
inline T EnumAdvance(const T e, const U n)
{
	return EnumAdvance(e, static_cast<int>(n));
}

}

#endif /* LIBUTIL_MISC_H_ */
