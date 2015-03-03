/*
 * ab_encoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(soft_quad_decoder)
#if MK60DZ10 || MK60D10 || MK60F15
#include LIBBASE_H(ftm_quad_decoder)
#endif

#include "libsc/config.h"
#include "libsc/ab_encoder.h"

using namespace LIBBASE_NS;

namespace libsc
{

#if LIBSC_USE_ENCODER

Encoder::QuadDecoder::Config AbEncoder::Initializer::GetQuadDecoderConfig() const
{
	Encoder::QuadDecoder::Config product =
			Encoder::Initializer::GetQuadDecoderConfig();
	product.encoding_mode =
			Encoder::QuadDecoder::Config::EncodingMode::kPhaseAB;
	return product;
}

AbEncoder::AbEncoder(const Config &config)
		: Encoder(Initializer(config))
{}

#else /* LIBSC_USE_ENCODER */
AbEncoder::AbEncoder(const Config&)
		: Encoder(nullptr)
{}

#endif /* LIBSC_USE_ENCODER */

}
