/*
 * soft_spi_master.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/sim.h"
#include "libbase/k60/soft_spi_master.h"
#include "libbase/k60/spi_master.h"

#include "libutil/misc.h"

using namespace libutil;
using namespace std;

namespace libbase
{
namespace k60
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

Gpo::Config GetCsConfig(const Pin::Name pin, const bool is_active_high)
{
	Gpo::Config config;
	config.pin = pin;
	config.is_high = (!is_active_high);
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

	for (Uint i = 0; i < kSlaveCount; ++i)
	{
		if (config.slaves[i].cs_pin == Pin::Name::kDisable)
		{
			break;
		}
		m_cs[i] = Gpo(GetCsConfig(config.slaves[i].cs_pin,
				config.slaves[i].is_active_high));
	}
}

uint16_t SoftSpiMaster::ExchangeData(const uint8_t slave_id, const uint16_t data)
{
	STATE_GUARD(SoftSpiMaster, 0);
	if (slave_id >= kSlaveCount || !m_cs[slave_id])
	{
		assert(false);
		return 0;
	}

	uint16_t receive = 0;
	m_cs[slave_id].Turn();
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
	m_cs[slave_id].Turn();
	return receive;
}

}
}
