/*
 * ab_encoder.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/config.h"
#include "libsc/encoder.h"

namespace libsc
{

/**
 * Phase A/B encoder. Provides two mechanisms to read the feedback value,
 * interrupt based software emulation and hardware quadrature decoding. Software
 * emulation will pose a large overhead to the system
 */
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
