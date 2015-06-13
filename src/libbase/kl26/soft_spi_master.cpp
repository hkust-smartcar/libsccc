/*
 * soft_spi_master.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/kl26/gpio.h"
#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/soft_spi_master.h"

#include "libutil/misc.h"

using namespace libutil;
using namespace std;

namespace libbase
{
namespace kl26
{

namespace
{

Gpi::Config GetSinConfig(const Pin::Name pin)
{
	Gpi::Config config;
	config.pin = pin;
	return config;
}

Gpo::Config GetSoutConfig(const Pin::Name pin)
{
	Gpo::Config config;
	config.pin = pin;
	config.is_high = false;
	return config;
}

Gpo::Config GetSckConfig(const Pin::Name pin, const bool is_sck_idle_low)
{
	Gpo::Config config;
	config.pin = pin;
	config.is_high = (!is_sck_idle_low);
	return config;
}

Gpo::Config GetCsConfig(const Pin::Name pin)
{
	Gpo::Config config;
	config.pin = pin;
	config.is_high = true;
	return config;
}

}

SoftSpiMaster::SoftSpiMaster(const Config &config)
		: m_sin(nullptr),
		  m_sout(nullptr),
		  m_sck(nullptr),

		  m_frame_size(Clamp<uint8_t>(1, config.frame_size, 16)),
		  m_is_sck_capture_first(config.is_sck_capture_first),
		  m_is_msb_first(config.is_msb_firt),

		  m_is_init(false)
{
	if (config.sin_pin == Pin::Name::kDisable
			&& config.sout_pin == Pin::Name::kDisable)
	{
		assert(false);
		return;
	}

	m_is_init = true;

	if (config.sin_pin != Pin::Name::kDisable)
	{
		m_sin = Gpi(GetSinConfig(config.sin_pin));
	}
	if (config.sout_pin != Pin::Name::kDisable)
	{
		m_sout = Gpo(GetSoutConfig(config.sout_pin));
	}
	m_sck = Gpo(GetSckConfig(config.sck_pin, config.is_sck_idle_low));
	m_cs = Gpo(GetCsConfig(config.pcs_pin));
}

uint16_t SoftSpiMaster::ExchangeData(const uint8_t slave_id, const uint16_t data)
{
	STATE_GUARD(SoftSpiMaster, 0);
	assert((slave_id == 0));

	uint16_t receive = 0;
	m_cs.Turn();
	if (!m_is_sck_capture_first)
	{
		m_sck.Turn();
	}
	for (int i = 0; i < m_frame_size; ++i)
	{
		const int i_ = m_is_msb_first ? (m_frame_size - i - 1) : i;
		if (m_sout)
		{
			m_sout.Set(GET_BIT(data, i_));
		}
		m_sck.Turn();
		if (m_sin && m_sin.Get())
		{
			SET_BIT(receive, i_);
		}
		// if !m_is_sck_capture_first, we don't need to turn at the end
		if (m_is_sck_capture_first || i != m_frame_size - 1)
		{
			m_sck.Turn();
		}
	}
	m_cs.Turn();
	return receive;
}

size_t SoftSpiMaster::PushData(const uint8_t, const uint16_t*, const size_t)
{
	return 0;
}

size_t SoftSpiMaster::PushData(const uint8_t, const uint8_t*, const size_t)
{
	return 0;
}

}
}
