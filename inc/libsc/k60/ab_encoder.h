/*
 * ab_encoder.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/config.h"
#include "libsc/k60/encoder.h"

namespace libsc
{
namespace k60
{

class AbEncoder : public Encoder
{
public:
	typedef Encoder::Config Config;

	explicit AbEncoder(const Config &config);

protected:
	struct Initializer : public Encoder::Initializer
	{
		explicit Initializer(const Config &config)
				: Encoder::Initializer(config)
		{}

		QuadDecoder::Config GetQuadDecoderConfig() const override;
	};
};

}
}
