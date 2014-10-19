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

}

uint32_t htobe32(const uint32_t host_32bits)
{
#if BYTE_ORDER == BIG_ENDIAN
	return host_32bits;
#else
	return ReverseUint32(host_32bits);
#endif
}

uint32_t htole32(const uint32_t host_32bits)
{
#if BYTE_ORDER == BIG_ENDIAN
	return ReverseUint32(host_32bits);
#else
	return host_32bits;
#endif
}

uint32_t be32toh(const uint32_t big_endian_32bits)
{
#if BYTE_ORDER == BIG_ENDIAN
	return big_endian_32bits;
#else
	return ReverseUint32(big_endian_32bits);
#endif
}

uint32_t le32toh(const uint32_t little_endian_32bits)
{
#if BYTE_ORDER == BIG_ENDIAN
	return ReverseUint32(little_endian_32bits);
#else
	return little_endian_32bits;
#endif
}
