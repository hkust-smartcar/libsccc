/*
 * quad_decoder_interface.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/kl26/pin.h"

namespace libbase
{
namespace kl26
{

/**
 * Generic quadrature decoder interface
 */
class QuadDecoderInterface
{
public:
	struct Config
	{
		Pin::Name a_pin;
		Pin::Name b_pin;
		bool is_invert_a_polarity = false;
		bool is_invert_b_polarity = false;
	};

	virtual ~QuadDecoderInterface()
	{}

	virtual operator bool() const = 0;

	virtual int32_t GetCount() = 0;
	virtual void ResetCount() = 0;
};

}
}
