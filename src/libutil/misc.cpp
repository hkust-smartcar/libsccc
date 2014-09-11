/*
 * misc.cpp
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libbase/syscall.h"
#include "libbase/k60/misc_utils.h"

#include "libsc/k60/uart_device.h"
#include "libutil/misc.h"

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

}

void InitDefaultFwriteHandler(UartDevice *uart)
{
	g_uart = uart;
	g_fwrite_handler = MyFwriteHandler;
}

void UninitDefaultFwriteHandler()
{
	g_uart = nullptr;
	g_fwrite_handler = nullptr;
}

}
