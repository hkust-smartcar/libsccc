/*
 * spi_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/spi.h"

namespace libbase
{
namespace kl26
{

class SpiUtils
{
public:
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
