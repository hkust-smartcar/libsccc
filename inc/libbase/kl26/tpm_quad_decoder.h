/*
 * tpm_quad_decoder.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/kl26/gpio.h"
#include "libbase/kl26/quad_decoder_interface.h"
#include "libbase/kl26/tpm_counter.h"

namespace libbase
{
namespace kl26
{

class TpmQuadDecoder : public QuadDecoderInterface
{
public:
	struct Config : public QuadDecoderInterface::Config
	{
		/**
		 * @see libbase::k60::FtmQuadDecoder::Config::EncodingMode
		 */
		enum struct EncodingMode
		{
			kPhaseAB,
			kCountDirection,
		};

		EncodingMode encoding_mode;
		uint8_t tpm_module;
	};

	explicit TpmQuadDecoder(const Config &config);
	explicit TpmQuadDecoder(nullptr_t);
	TpmQuadDecoder(const TpmQuadDecoder&) = delete;
	TpmQuadDecoder(TpmQuadDecoder &&rhs);
	~TpmQuadDecoder();

	TpmQuadDecoder& operator=(const TpmQuadDecoder&) = delete;
	TpmQuadDecoder& operator=(TpmQuadDecoder &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	int32_t GetCount() override;
	void ResetCount() override;

private:
	void Uninit();

	void OnTick(Gpi*, const bool is_revert);

	bool m_is_invert_b_polarity;
	bool m_is_dir_mode;

	TpmCounter m_counter;
	Gpi m_qdb;

	volatile int32_t m_count;

	bool m_is_init;
};

}
}
