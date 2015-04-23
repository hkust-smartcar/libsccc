/*
 * us_100.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(uart)

#include "libsc/config.h"
#include "libsc/us_100.h"

using namespace LIBBASE_NS;
using namespace std;

namespace libsc
{

#ifdef LIBSC_USE_US100

namespace
{

#if LIBSC_USE_US100 == 1
inline Pin::Name GetTxPin(const uint8_t)
{
	return LIBSC_US1000_TX;
}

inline Pin::Name GetRxPin(const uint8_t)
{
	return LIBSC_US1000_RX;
}

#else
inline Pin::Name GetTxPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_US1000_TX;

	case 1:
		return LIBSC_US1001_TX;
	}
}

inline Pin::Name GetRxPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_US1000_RX;

	case 1:
		return LIBSC_US1001_RX;
	}
}

#endif /* LIBSC_USE_US100 == 1 */

inline Uart::Config GetUartConfig(const Us100::Config &config,
		const Uart::OnRxFullListener &rx_isr)
{
	Uart::Config product;
	product.rx_pin = GetRxPin(config.id);
	product.tx_pin = GetTxPin(config.id);
	product.baud_rate = Uart::Config::BaudRate::k9600;
	product.rx_isr = rx_isr;
	return product;
}

}

Us100::Us100(const Config &config)
		: m_uart(GetUartConfig(config, std::bind(&Us100::OnRxFull, this,
				placeholders::_1))),
		  m_distance(0),
		  m_buffer{},
		  m_buffer_it(0),
		  m_is_shoot(false),
		  m_is_shooting(false),
		  m_is_available(false)
{
	m_uart.SetEnableRxIrq(true);
}

Us100::~Us100()
{}

void Us100::Start()
{
	m_is_available = false;
	m_is_shoot = true;
	if (!m_is_shooting)
	{
		Shoot();
	}
}

void Us100::Stop()
{
	m_is_available = false;
	m_is_shoot = false;
}

void Us100::OnRxFull(Uart *uart)
{
	Byte byte;
	if (!uart->PeekByte(&byte))
	{
		return;
	}

	m_buffer[m_buffer_it++] = byte;
	if (m_buffer_it == 2)
	{
		const uint16_t dist = (m_buffer[0] << 8) | m_buffer[1];
		m_distance = dist;
		m_buffer_it = 0;
		m_is_available = true;

		if (m_is_shoot)
		{
			Shoot();
		}
		else
		{
			m_is_shooting = false;
		}
	}
}

void Us100::Shoot()
{
	m_is_shooting = true;
	m_uart.SendByte(0x55);
}

#else
Us100::Us100(const Config&)
		: m_uart(nullptr), m_distance(0), m_buffer{}, m_buffer_it(0),
		  m_is_shoot(false), m_is_shooting(false), m_is_available(false)
{
	LOG_DL("Configured not to use Us100");
}
Us100::~Us100() {}
void Us100::Start() {}
void Us100::Stop() {}

#endif /* LIBSC_USE_US100 */

}
