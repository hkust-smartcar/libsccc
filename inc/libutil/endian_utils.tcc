/*
 * endian_utils.tcc
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libutil/endian_utils.h"

namespace libutil
{

namespace internal
{

template<typename T, size_t T_SIZE=sizeof(T)>
struct Translate_
{
	static T Do(const T from);
};

template<typename T>
struct Translate_<T, 2>
{
	static T Do(const T from)
	{
		return EndianUtils::Translate16(from);
	}
};

template<typename T>
struct Translate_<T, 4>
{
	static T Do(const T from)
	{
		return EndianUtils::Translate32(from);
	}
};

}

template<typename T>
T EndianUtils::Translate(const T from)
{
	return internal::Translate_<T>::Do(from);
}

template<typename T>
T EndianUtils::HostToBe(const T host)
{
	if (IsLittleEndian())
	{
		return Translate(host);
	}
	else
	{
		return host;
	}
}

template<typename T>
T EndianUtils::HostToLe(const T host)
{
	if (IsBigEndian())
	{
		return Translate(host);
	}
	else
	{
		return host;
	}
}

}
