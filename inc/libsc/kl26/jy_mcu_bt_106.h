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

#include "libbase/kl26/uart.h"

#include "libsc/kl26/uart_device.h"

namespace libsc
{
namespace kl26
{

class JyMcuBt106 : public UartDevice
{
public:
	typedef UartDevice::Config Config;

	explicit JyMcuBt106(const Config &config);

protected:
	struct Initializer : public UartDevice::Initializer
	{
		explicit Initializer(const Config &config)
				: UartDevice::Initializer(config)
		{}

		libbase::kl26::Uart::Config GetUartConfig() const override;
	};
};

}
}
