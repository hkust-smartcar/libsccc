/*
 * endian_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>
#include <algorithm>

#include "libutil/endian_utils.h"
#include "libutil/misc.h"

namespace libutil
{

namespace
{

bool IsBigEndian_()
{
	const uint16_t bom = 0xFFFE;
	return (reinterpret_cast<const Byte*>(&bom)[0] == 0xFF);
}

}

bool EndianUtils::IsBigEndian()
{
	static const bool is_be = IsBigEndian_();
	return is_be;
}

uint16_t EndianUtils::Translate16(const uint16_t from)
{
	uint16_t to = from;
	Byte *bytes = reinterpret_cast<Byte*>(&to);
	std::swap(bytes[0], bytes[1]);
	return to;
}

uint32_t EndianUtils::Translate32(const uint32_t from)
{
	uint32_t to = from;
	Byte *bytes = reinterpret_cast<Byte*>(&to);
	std::swap(bytes[0], bytes[3]);
	std::swap(bytes[1], bytes[2]);
	return to;
}

}
