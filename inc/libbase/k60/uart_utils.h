/*
 * uart_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/uart.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace k60
{

class UartUtils
{
public:
	static Uint GetUartModule(const Uart::Name uart)
	{
		return static_cast<Uint>(uart) / 2;
	}
};

}
}
