/*
 * spi_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/spi.h"

namespace libbase
{
namespace k60
{

class SpiUtils
{
public:
	static Uint GetCsNumber(const Pin::Name pin);

	static Uint GetSpiModule(const Spi::PcsName pcs)
	{
		return static_cast<Uint>(pcs) / Spi::GetMaxSpiPcsCount();
	}

	static Uint GetSpiPcs(const Spi::PcsName pcs)
	{
		return static_cast<Uint>(pcs) % Spi::GetMaxSpiPcsCount();
	}
};

}
}
