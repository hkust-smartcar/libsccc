/*
 * jy_mcu_bt_106.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/uart.h"

#include "libsc/k60/jy_mcu_bt_106.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

JyMcuBt106::JyMcuBt106(const uint8_t id,
		const Uart::Config::BaudRate baud_rate)
		: UartDevice(UartConfigBuilder(id, baud_rate, this))
{}

Uart::Config JyMcuBt106::UartConfigBuilder::Build() const
{
	Uart::Config config = UartDevice::UartConfigBuilder::Build();
	// On this board, there's a diode connected to the Tx pin, preventing the
	// module to correctly send data to the MCU
	config.rx_config[Pin::Config::kPullEnable] = true;
	config.rx_config[Pin::Config::kPullUp] = true;
	return config;
}

}
}
