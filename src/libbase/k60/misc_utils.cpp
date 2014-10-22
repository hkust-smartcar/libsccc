/*
 * misc_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include <algorithm>

#include <machine/endian.h>

#include "libbase/k60/misc_utils.h"

namespace
{

uint32_t ReverseUint32(const uint32_t x)
{
	uint32_t result = x;
	Byte *data = reinterpret_cast<Byte*>(&result);
	std::reverse(data, data + 4);
	return result;
}

bool IsBigEndian_()
{
	const uint16_t bom = 0xFFFE;
	return (reinterpret_cast<const Byte*>(&bom)[0] == 0xFF);
}

bool IsBigEndian()
{
	static const bool is_be = IsBigEndian_();
	return is_be;
}

}

uint32_t htobe32(const uint32_t host_32bits)
{
	if (!IsBigEndian())
	{
		return ReverseUint32(host_32bits);
	}
	else
	{
		return host_32bits;
	}
}

uint32_t htole32(const uint32_t host_32bits)
{
	if (IsBigEndian())
	{
		return ReverseUint32(host_32bits);
	}
	else
	{
		return host_32bits;
	}
}

uint32_t be32toh(const uint32_t big_endian_32bits)
{
	if (!IsBigEndian())
	{
		return ReverseUint32(big_endian_32bits);
	}
	else
	{
		return big_endian_32bits;
	}
}

uint32_t le32toh(const uint32_t little_endian_32bits)
{
	if (IsBigEndian())
	{
		return ReverseUint32(little_endian_32bits);
	}
	else
	{
		return little_endian_32bits;
	}
}
