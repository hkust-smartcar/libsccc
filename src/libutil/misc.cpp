/*
 * misc.cpp
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libbase/helper.h"
#include "libbase/syscall.h"

#if MK60DZ10 || MK60D10 || MK60F15
#include "libsc/k60/uart_device.h"

#elif MKL26Z4
#include "libsc/kl26/uart_device.h"

#endif

#include "libutil/misc.h"

#if MK60DZ10 || MK60D10 || MK60F15
using namespace libsc::k60;

#elif MKL26Z4
using namespace libsc::kl26;

#endif

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
