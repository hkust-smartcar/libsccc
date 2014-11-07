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

void JyMcuBt106::OnConfigUart(Uart::Config *config)
{
	// On this board, there's a diode connected to the Tx pin, preventing the
	// module to correctly send data to the MCU
	config->rx_config[Pin::Config::kPullEnable] = true;
	config->rx_config[Pin::Config::kPullUp] = true;
}

}
}
