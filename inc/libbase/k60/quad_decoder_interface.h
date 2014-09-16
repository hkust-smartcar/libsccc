/*
 * quad_decoder_interface.h
 * Generic quadrature decoder interface
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

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
