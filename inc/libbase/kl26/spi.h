/*
 * spi.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/kl26/pinout_macros.h"

namespace libbase
{
namespace kl26
{

class Spi
{
public:
	enum struct MisoName
	{
		kSpi0Miso = 0,
		kSpi1Miso,
		kDisable
	};

	enum struct MosiName
	{
		kSpi0Mosi = 0,
		kSpi1Mosi,
		kDisable
	};

	enum struct SckName
	{
		kSpi0Sck = 0,
		kSpi1Sck,
		kDisable
	};

	enum struct PcsName
	{
		kSpi0Pcs0 = 0,
		kSpi1Pcs0,
		kDisable
	};

	static constexpr int GetMaxSpiPcsCount()
	{
		return static_cast<int>(PcsName::kSpi1Pcs0);
	}
};

}
}
