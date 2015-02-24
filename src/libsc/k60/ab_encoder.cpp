/*
 * ab_encoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libbase/k60/ftm_quad_decoder.h"

#include "libsc/config.h"
#include "libsc/k60/ab_encoder.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#if LIBSC_USE_ENCODER

#if LIBSC_USE_SOFT_ENCODER
SoftQuadDecoder::Config AbEncoder::Initializer::GetQuadDecoderConfig() const
{
	return Encoder::Initializer::GetQuadDecoderConfig();
}

#else
FtmQuadDecoder::Config AbEncoder::Initializer::GetQuadDecoderConfig() const
{
	FtmQuadDecoder::Config product = Encoder::Initializer::GetQuadDecoderConfig();
	product.encoding_mode = FtmQuadDecoder::Config::EncodingMode::kPhaseAB;
	return product;
}

#endif

AbEncoder::AbEncoder(const Config &config)
		: Encoder(Initializer(config))
{}

#else /* LIBSC_USE_ENCODER */
AbEncoder::AbEncoder(const Config&)
		: Encoder(nullptr)
{}

#endif /* LIBSC_USE_ENCODER */

}
}
