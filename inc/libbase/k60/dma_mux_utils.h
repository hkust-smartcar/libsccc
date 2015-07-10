/*
 * dma_mux_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class DmaMuxUtils
{
public:
	static Uint GetModule(const Uint ch)
	{
		return static_cast<Uint>(ch) >> 4;
	}

	static Uint GetChannel(const Uint ch)
	{
		return static_cast<Uint>(ch) & 0xF;
	}
};

}
}
