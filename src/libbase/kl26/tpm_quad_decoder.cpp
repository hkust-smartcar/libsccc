/*
 * tpm_quad_decoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/kl26/gpio.h"
#include "libbase/kl26/quad_decoder_interface.h"
#include "libbase/kl26/tpm_counter.h"
#include "libbase/kl26/tpm_quad_decoder.h"

namespace libbase
{
namespace kl26
{

namespace
{

TpmCounter::Config GetTpmCounterConfig(const TpmQuadDecoder::Config &config)
{
	TpmCounter::Config product;
	product.clockin_pin = config.a_pin;
	product.module = config.tpm_module;
	return product;
}

Gpi::Config GetQdbConfig(const TpmQuadDecoder::Config &config,
		const Gpi::OnGpiEventListener &isr)
{
	Gpi::Config product;
	product.pin = config.b_pin;
	product.interrupt = Pin::Config::Interrupt::kBoth;
	product.isr = isr;
	return product;
}

}

TpmQuadDecoder::TpmQuadDecoder(const Config &config)
		: m_is_invert_b_polarity(config.is_invert_b_polarity),
		  m_is_dir_mode(config.encoding_mode
				  == Config::EncodingMode::kCountDirection),
		  m_counter(GetTpmCounterConfig(config)),
		  m_qdb(GetQdbConfig(config, std::bind(&TpmQuadDecoder::OnTick, this,
				  std::placeholders::_1, true))),
		  m_count(0),
		  m_is_init(true)
{
	// TpmQuadDecoder only works with DIR encoder
	assert(m_is_dir_mode);
}

TpmQuadDecoder::TpmQuadDecoder(TpmQuadDecoder &&rhs)
		: TpmQuadDecoder(nullptr)
{
	*this = std::move(rhs);
}

TpmQuadDecoder::TpmQuadDecoder(nullptr_t)
		: m_is_invert_b_polarity(false),
		  m_is_dir_mode(false),
		  m_counter(nullptr),
		  m_qdb(nullptr),
		  m_count(0),
		  m_is_init(false)
{}

TpmQuadDecoder::~TpmQuadDecoder()
{
	Uninit();
}

TpmQuadDecoder& TpmQuadDecoder::operator=(TpmQuadDecoder &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_is_invert_b_polarity = rhs.m_is_invert_b_polarity;
			m_is_dir_mode = rhs.m_is_dir_mode;

			m_counter = std::move(rhs.m_counter);
			m_qdb = std::move(rhs.m_qdb);

			m_count = rhs.m_count;

			m_is_init = true;
		}
	}
	return *this;
}

void TpmQuadDecoder::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		m_counter = TpmCounter(nullptr);
		m_qdb = Gpi(nullptr);
	}
}

int32_t TpmQuadDecoder::GetCount()
{
	OnTick(nullptr, false);
	if (!m_is_dir_mode)
	{
		return m_count * 4;
	}
	else
	{
		return m_count;
	}
}

void TpmQuadDecoder::ResetCount()
{
	m_count = 0;
}

void TpmQuadDecoder::OnTick(Gpi*, const bool is_revert)
{
	m_counter.Update();
	// Under dir mode, the relation between qdb and dir is inverted
	if ((!(m_qdb.Get() ^ m_is_invert_b_polarity)) ^ m_is_dir_mode ^ !is_revert)
	{
		m_count -= m_counter.GetCount();
	}
	else
	{
		m_count += m_counter.GetCount();
	}
}

}
}
