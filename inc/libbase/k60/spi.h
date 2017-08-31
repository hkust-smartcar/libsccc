/*
 * spi.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

namespace libbase
{
namespace k60
{

class Spi
{
public:
	enum struct SinName
	{
		kSpi0Sin = 0,
		kSpi1Sin,
		kSpi2Sin,
		kDisable
	};

	enum struct SoutName
	{
		kSpi0Sout = 0,
		kSpi1Sout,
		kSpi2Sout,
		kDisable
	};

	enum struct SckName
	{
		kSpi0Sck = 0,
		kSpi1Sck,
		kSpi2Sck,
		kDisable
	};

	enum struct PcsName
	{
		kSpi0Pcs0 = 0,
		kSpi0Pcs1,
		kSpi0Pcs2,
		kSpi0Pcs3,
		kSpi0Pcs4,
		kSpi0Pcs5,

		kSpi1Pcs0 = 6,
		kSpi1Pcs1,
		kSpi1Pcs2,
		kSpi1Pcs3,

		kSpi2Pcs0 = 12,
		kSpi2Pcs1,

		kDisable = 18
	};

	static constexpr int GetMaxSpiPcsCount()
	{
		return static_cast<int>(PcsName::kSpi1Pcs0);
	}
};

}
}
