/*
 * al422b.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>

#include <vector>

#include "libbase/misc_types.h"
#include "libbase/helper.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(gpio_array)

#include "libsc/config.h"
#include "libsc/k60/al422b.h"
#include "libsc/system.h"

using namespace LIBBASE_NS;
using namespace std;

namespace
{

Gpo::Config GetWeConfig(const Pin::Name we)
{
	Gpo::Config product;
	product.pin = we;
	product.is_high = false;
	return product;
}

Gpo::Config GetWrstConfig(const Pin::Name wrst)
{
	Gpo::Config product;
	product.pin = wrst;
	product.config.set(Pin::Config::kHighDriveStrength);
	product.is_high = false;
	return product;
}

GpiArray::Config GetGpiArrayConfig(const Pin::Name do0)
{
	GpiArray::Config product;
	product.start_pin = do0;
	product.count = 8;
	return product;
}

Gpo::Config GetRclkConfig(const Pin::Name rck)
{
	Gpo::Config product;
	product.pin = rck;
	product.config.set(Pin::Config::kHighDriveStrength);
	product.is_high = false;
	return product;
}

Gpo::Config GetReConfig(const Pin::Name re)
{
	Gpo::Config product;
	product.pin = re;
	product.is_high = false;
	return product;
}

Gpo::Config GetRrstConfig(const Pin::Name rrst)
{
	Gpo::Config product;
	product.pin = rrst;
	product.config.set(Pin::Config::kHighDriveStrength);
	product.is_high = false;
	return product;
}

Gpo::Config GetOeConfig(const Pin::Name oe)
{
	Gpo::Config product;
	product.pin = oe;
	product.is_high = false;
	return product;
}

}

namespace libsc
{
namespace k60
{

Al422b::Al422b(const Config &config)
		: m_wrst(GetWrstConfig(config.wrst)),
		  m_do(GetGpiArrayConfig(config.do0)),
		  m_rck(GetRclkConfig(config.rck)),
		  m_rrst(GetRclkConfig(config.rrst)),
		  m_data(config.initial_size),
		  m_it(m_data.end())
{
	assert(!(config.initial_size % 8));
	if (config.we != Pin::Name::kDisable)
	{
		m_we = Gpo(GetWeConfig(config.we));
	}
	if (config.re != Pin::Name::kDisable)
	{
		m_re = Gpo(GetReConfig(config.re));
	}
	if (config.oe != Pin::Name::kDisable)
	{
		m_oe = Gpo(GetOeConfig(config.oe));
	}
	System::DelayUs(100);
}

Al422b::~Al422b()
{}

void Al422b::Start(const uint32_t size)
{
	m_data.resize(size);
	m_it = m_data.begin();
}

bool Al422b::ReadStep()
{
	if (m_it == m_data.end())
	{
		return true;
	}

	m_rck.Set();
	m_do.Get(&(*m_it++), 1);
	m_rck.Reset();
	return (m_it == m_data.end());
}

void Al422b::ResetWrite()
{
	m_wrst.Reset();
	m_wrst.Set();
}

void Al422b::ResetRead()
{
	m_rrst.Reset();
	m_rck.Reset();
	// Reset cycle
	m_rck.Set();
	m_rck.Reset();
	m_rrst.Set();
}

}
}
