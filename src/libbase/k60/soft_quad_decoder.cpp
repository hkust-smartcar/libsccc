/*
 * soft_quad_decoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/k60/gpio.h"
#include "libbase/k60/quad_decoder.h"
#include "libbase/k60/soft_quad_decoder.h"

namespace libbase
{
namespace k60
{

namespace
{

Gpi::Config GetQdaConfig(const SoftQuadDecoder::Config &config,
		const Gpi::OnGpiEventListener &isr)
{
	Gpi::Config gc;
	gc.pin = config.a_pin;
	gc.interrupt = Pin::Config::Interrupt::kRising;
	gc.isr = isr;
	return gc;
}

Gpi::Config GetQdbConfig(const SoftQuadDecoder::Config &config)
{
	Gpi::Config gc;
	gc.pin = config.b_pin;
	return gc;
}

}

SoftQuadDecoder::SoftQuadDecoder(const Config &config)
		: m_is_invert_a_polarity(config.is_invert_a_polarity),
		  m_is_invert_b_polarity(config.is_invert_b_polarity),
		  m_qda(GetQdaConfig(config, std::bind(&SoftQuadDecoder::OnTick, this,
				  std::placeholders::_1))),
		  m_qdb(GetQdbConfig(config)),
		  m_count(0),
		  m_is_init(true)
{}

SoftQuadDecoder::SoftQuadDecoder(SoftQuadDecoder &&rhs)
		: SoftQuadDecoder(nullptr)
{
	*this = std::move(rhs);
}

SoftQuadDecoder::SoftQuadDecoder(nullptr_t)
		: m_is_invert_a_polarity(false),
		  m_is_invert_b_polarity(false),
		  m_qda(nullptr),
		  m_qdb(nullptr),
		  m_count(0),
		  m_is_init(false)
{}

SoftQuadDecoder::~SoftQuadDecoder()
{
	Uninit();
}

SoftQuadDecoder& SoftQuadDecoder::operator=(SoftQuadDecoder &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_is_invert_a_polarity = rhs.m_is_invert_a_polarity;
			m_is_invert_b_polarity = rhs.m_is_invert_b_polarity;

			m_qda = std::move(rhs.m_qda);
			m_qdb = std::move(rhs.m_qdb);

			m_count = rhs.m_count;

			m_is_init = true;
		}
	}
	return *this;
}

void SoftQuadDecoder::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		m_qda = Gpi(nullptr);
		m_qdb = Gpi(nullptr);
	}
}

int32_t SoftQuadDecoder::GetCount()
{
	return m_count * 4;
}

void SoftQuadDecoder::ResetCount()
{
	m_count = 0;
}

void SoftQuadDecoder::OnTick(Gpi*)
{
	if (!(m_qdb.Get() ^ m_is_invert_b_polarity))
	{
		++m_count;
	}
	else
	{
		--m_count;
	}
}

}
}
