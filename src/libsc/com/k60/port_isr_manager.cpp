/*
 * port_isr_manager.cpp
 * Manage ISR for ports
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstring>

#include <MK60_gpio.h>
#include <vectors.h>

#include "libsc/com/k60/port_isr_manager.h"

namespace libsc
{

PortIsrManager* PortIsrManager::m_instance = nullptr;

PortIsrManager* PortIsrManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new PortIsrManager;
	}
	return m_instance;
}

PortIsrManager::PortIsrManager()
{
	for (int i = 0; i < PORT_COUNT; ++i)
	{
		m_handlers[i] = nullptr;
		m_is_enabled[i] = false;
	}
}

PortIsrManager::~PortIsrManager()
{
	for (int i = 0; i < PORT_COUNT; ++i)
	{
		if (m_is_enabled[i])
		{
			const VECTORn_t v = static_cast<VECTORn_t>(PORTA_VECTORn + i);
			SetIsr(v, DefaultIsr);
			DisableIsr(v);
		}
		if (m_handlers[i])
		{
			delete[] m_handlers[i];
		}
	}
}

void PortIsrManager::InitPort(const Uint port)
{
	if (!m_handlers[port])
	{
		m_handlers[port] = new tIsrFunc[PIN_COUNT];
		memset(m_handlers[port], 0, PIN_COUNT * sizeof(tIsrFunc));
	}

	switch (port)
	{
	case PTA:
		SetIsr(PORTA_VECTORn, IsrHandler<PTA>);
		break;

	case PTB:
		SetIsr(PORTB_VECTORn, IsrHandler<PTB>);
		break;

	case PTC:
		SetIsr(PORTC_VECTORn, IsrHandler<PTC>);
		break;

	case PTD:
		SetIsr(PORTD_VECTORn, IsrHandler<PTD>);
		break;

	case PTE:
		SetIsr(PORTE_VECTORn, IsrHandler<PTE>);
		break;

	default:
		return;
	}
	EnableIsr(static_cast<VECTORn_t>(PORTA_VECTORn + port));
	m_is_enabled[port] = true;
}

template<PTX_e port>
__ISR void PortIsrManager::IsrHandler()
{
	tIsrFunc *port_handlers = PortIsrManager::GetInstance()->m_handlers[port];
	for (int i = 0; i < PIN_COUNT; ++i)
	{
		if ((PORT_ISFR_REG(PORTX[port]) & (1 << i)))
		{
			PORT_ISFR_REG(PORTX[port]) |= (1 << i);
			if (port_handlers[i])
			{
				port_handlers[i]();
			}
		}
	}
}

void PortIsrManager::SetIsrHandler(const Uint port, const Uint pin,
		tIsrFunc fn)
{
	if (fn)
	{
		if (!m_is_enabled[port])
		{
			InitPort(port);
		}
		m_handlers[port][pin] = fn;
	}
	else if (m_is_enabled[port])
	{
		m_handlers[port][pin] = nullptr;

		// Disable interrupt if all are null
		for (int i = 0; i < PIN_COUNT; ++i)
		{
			if (m_handlers[port][i])
			{
				return;
			}
		}
		const VECTORn_t v = static_cast<VECTORn_t>(PORTA_VECTORn + port);
		SetIsr(v, DefaultIsr);
		DisableIsr(v);
		m_is_enabled[port] = false;
	}
}

}
