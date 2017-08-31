/*
 * dir_encoder.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libsc/config.h"
#include "libsc/encoder.h"

namespace libsc
{

class DirEncoder : public Encoder
{
public:
	typedef Encoder::Config Config;

	explicit DirEncoder(const Config &config);

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
