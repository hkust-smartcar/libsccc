/*
 * ftdi_ft232r.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libsc/config.h"
#include "libsc/kl26/ftdi_ft232r.h"

namespace libsc
{
namespace kl26
{

#ifdef LIBSC_USE_UART
FtdiFt232r::FtdiFt232r(const Config &config)
		: UartDevice(Initializer(config))
{}

#else /* LIBSC_USE_UART */
FtdiFt232r::FtdiFt232r(const Config&)
		: UartDevice(nullptr)
{}

#endif /* LIBSC_USE_UART */

}
}
