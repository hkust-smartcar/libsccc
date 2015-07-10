/*
 * endian_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

namespace libutil
{

class EndianUtils
{
public:
	static bool IsBigEndian();
	static bool IsLittleEndian()
	{
		return !IsBigEndian();
	}

	template<typename T>
	static T Translate(const T from);
	static uint16_t Translate16(const uint16_t from);
	static uint32_t Translate32(const uint32_t from);

	/**
	 * Host to big-endian encoding
	 *
	 * @param host Host byte order data
	 * @return Big-endian data
	 */
	template<typename T>
	static T HostToBe(const T host);
	/**
	 * Host to little-endian encoding
	 *
	 * @param host Host byte order data
	 * @return Little-endian data
	 */
	template<typename T>
	static T HostToLe(const T host);
};

}

#include "endian_utils.tcc"
