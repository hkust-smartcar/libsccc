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
	}
}

PortIsrManager::~PortIsrManager()
{
	for (int i = 0; i < PORT_COUNT; ++i)
	{
		if (m_handlers[i])
		{
			delete[] m_handlers[i];
		}
	}
}

void PortIsrManager::InitPort(const PTX_e port)
{
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
	m_handlers[port] = new tIsrFunc[PIN_COUNT];
	memset(m_handlers[port], 0, PIN_COUNT * sizeof(tIsrFunc));
	EnableIsr(static_cast<VECTORn_t>(PORTA_VECTORn + port));
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

void PortIsrManager::SetIsrHandler(const PTX_e port, const PTn_e pin,
		tIsrFunc fn)
{
	if (!m_handlers[port])
	{
		InitPort(port);
	}
	m_handlers[port][pin] = fn;
}

}
