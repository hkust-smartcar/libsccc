/*
 * ftdi_ft232r.h
 * Generic class for the FTDI FT232R USB UART IC
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/kl26/uart_device.h"

namespace libsc
{
namespace kl26
{

class FtdiFt232r : public UartDevice
{
public:
	typedef UartDevice::Config Config;

	explicit FtdiFt232r(const Config &config);
};

}
}
