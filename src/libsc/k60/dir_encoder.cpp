/*
 * dir_encoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/ftm_quad_decoder.h"

#include "libsc/config.h"
#include "libsc/k60/dir_encoder.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#if LIBSC_USE_ENCODER

Encoder::QuadDecoder::Config DirEncoder::Initializer::GetQuadDecoderConfig() const
{
	Encoder::QuadDecoder::Config product =
			Encoder::Initializer::GetQuadDecoderConfig();
	product.encoding_mode =
			Encoder::QuadDecoder::Config::EncodingMode::kCountDirection;
	return product;
}

DirEncoder::DirEncoder(const Config &config)
		: Encoder(Initializer(config))
{}

#else /* LIBSC_USE_ENCODER */
DirEncoder::DirEncoder(const Config&)
		: Encoder(nullptr)
{}

#endif /* LIBSC_USE_ENCODER */

}
}
