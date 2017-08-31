/*
 * misc.h
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <algorithm>

#include "libbase/helper.h"
#include LIBBASE_H(misc_utils)

#if MK60DZ10 || MK60D10 || MK60F15
namespace libsc
{
namespace k60
{

class UartDevice;

}
}

#elif MKL26Z4
namespace libsc
{
namespace kl26
{

class UartDevice;

}
}

#endif

#define SAFE_DELETE(x) do{if (x) {delete x; x = nullptr;}}while(false)

#define UTIL_JOIN(x, y) x ## y

namespace libutil
{

#if MK60DZ10 || MK60D10 || MK60F15
void InitDefaultFwriteHandler(libsc::k60::UartDevice *uart);

#elif MKL26Z4
void InitDefaultFwriteHandler(libsc::kl26::UartDevice *uart);

#endif

void UninitDefaultFwriteHandler();

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
