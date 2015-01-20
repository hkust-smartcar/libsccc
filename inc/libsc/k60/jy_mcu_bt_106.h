/*
 * jy_mcu_bt_106.h
 * JY-MCU BT Board v1.06
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/uart.h"

#include "libsc/k60/uart_device.h"

namespace libsc
{
namespace k60
{

class JyMcuBt106 : public UartDevice
{
public:
	struct Config
	{
		uint8_t id;
		libbase::k60::Uart::Config::BaudRate baud_rate;
	};

	explicit JyMcuBt106(const Config &config);
};

}
}
