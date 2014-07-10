/*
 * misc.cpp
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <syscall.h>

#include <cstdint>

#include <algorithm>

#include <machine/endian.h>

#include "libsc/k60/uart_device.h"

using namespace libsc::k60;

namespace libutil
{

namespace
{

UartDevice *g_uart = nullptr;

int MyFwriteHandler(int, char *ptr, int len)
{
	if (g_uart)
	{
		g_uart->SendBuffer(reinterpret_cast<const Byte*>(ptr), len);
	}
	return len;
}

uint32_t ReverseUint32(const uint32_t x)
{
	uint32_t result = x;
	Byte *data = reinterpret_cast<Byte*>(&result);
	std::reverse(data, data + 4);
	return result;
}

}

void InitDefaultFwriteHandler(UartDevice *uart)
{
	g_uart = uart;
	__g_fwrite_handler = MyFwriteHandler;
}

void UninitDefaultFwriteHandler()
{
	g_uart = nullptr;
	__g_fwrite_handler = nullptr;
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

}
