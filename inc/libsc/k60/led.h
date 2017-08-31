/*
 * led.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/dma.h"
#include "libbase/k60/gpio.h"

#include "libsc/led.h"

namespace libsc
{
namespace k60
{

class Led : public libsc::Led
{
public:
	using libsc::Led::Led;

	/**
	 * @param config
	 * @see Gpo::ConfigToggleAsDmaDst()
	 */
	void ConfigToggleAsDmaDst(libbase::k60::Dma::Config *config)
	{
		GetPin()->ConfigToggleAsDmaDst(config);
	}
};

}
}
