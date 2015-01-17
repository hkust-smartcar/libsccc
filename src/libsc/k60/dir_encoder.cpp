/*
 * dir_encoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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

#if LIBSC_USE_SOFT_ENCODER
SoftQuadDecoder::Config AbEncoder::Initializer::GetQuadDecoderConfig() const
{
	return Initializer::GetQuadDecoderConfig();
}

#else
FtmQuadDecoder::Config DirEncoder::Initializer::GetQuadDecoderConfig() const
{
	FtmQuadDecoder::Config product = Initializer::GetQuadDecoderConfig();
	product.encoding_mode =
			FtmQuadDecoder::Config::EncodingMode::kCountDirection;
	return product;
}

#endif

DirEncoder::DirEncoder(const Config &config)
		: Encoder(Initializer(config))
{}

}
}
