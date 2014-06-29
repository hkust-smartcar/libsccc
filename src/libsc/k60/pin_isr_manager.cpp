/*
 * pin_isr_manager.cpp
 * Manage ISR for ports
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <hw_common.h>
#include <cstring>

#include <vectors.h>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"

#include "libsc/k60/pin_isr_manager.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

PinIsrManager* PinIsrManager::m_instance = nullptr;

PinIsrManager* PinIsrManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new PinIsrManager;
	}
	return m_instance;
}

PinIsrManager::PinIsrManager()
{
	for (int i = 0; i < PinConfig::PORT_COUNT; ++i)
	{
		m_pin_data[i] = nullptr;
		m_is_enable[i] = false;
	}
}

PinIsrManager::~PinIsrManager()
{
	for (int i = 0; i < PinConfig::PORT_COUNT; ++i)
	{
		if (m_is_enable[i])
		{
			const VECTORn_t v = static_cast<VECTORn_t>(PORTA_VECTORn + i);
			SetIsr(v, DefaultIsr);
			DisableIsr(v);
		}
		if (m_pin_data[i])
		{
			delete[] m_pin_data[i];
		}
	}
}

void PinIsrManager::InitPort(const Uint port)
{
	if (!m_pin_data[port])
	{
		m_pin_data[port] = new PinData[PinConfig::PORT_PIN_COUNT];
		memset(m_pin_data[port], 0, PinConfig::PORT_PIN_COUNT * sizeof(PinData));
	}

	switch (port)
	{
	case 0:
		SetIsr(PORTA_VECTORn, PortIrqHandler<0>);
		break;

	case 1:
		SetIsr(PORTB_VECTORn, PortIrqHandler<1>);
		break;

	case 2:
		SetIsr(PORTC_VECTORn, PortIrqHandler<2>);
		break;

	case 3:
		SetIsr(PORTD_VECTORn, PortIrqHandler<3>);
		break;

	case 4:
		SetIsr(PORTE_VECTORn, PortIrqHandler<4>);
		break;

	default:
		return;
	}
	EnableIsr(static_cast<VECTORn_t>(PORTA_VECTORn + port));
	m_is_enable[port] = true;
}

void PinIsrManager::SetPinIsr(libbase::k60::Pin *pin, OnPinIrqListener isr)
{
	const Uint port = PinUtils::GetPort(pin->GetName());
	const Uint pin_num = PinUtils::GetPinNumber(pin->GetName());
	if (isr)
	{
		if (!m_is_enable[port])
		{
			InitPort(port);
		}
		m_pin_data[port][pin_num].pin = pin;
		m_pin_data[port][pin_num].isr = isr;
	}
	else if (m_is_enable[port])
	{
		m_pin_data[port][pin_num].pin = nullptr;
		m_pin_data[port][pin_num].isr = nullptr;

		// Disable interrupt only if all are null
		for (int i = 0; i < PinConfig::PORT_PIN_COUNT; ++i)
		{
			if (m_pin_data[port][i].isr)
			{
				return;
			}
		}
		const VECTORn_t v = static_cast<VECTORn_t>(PORTA_VECTORn + port);
		SetIsr(v, DefaultIsr);
		DisableIsr(v);
		m_is_enable[port] = false;
	}
}

template<Uint port>
__ISR void PinIsrManager::PortIrqHandler()
{
	PinData *pin_data = PinIsrManager::GetInstance()->m_pin_data[port];
	for (int i = 0; i < PinConfig::PORT_PIN_COUNT; ++i)
	{
		if (pin_data[i].pin->IsInterruptRequested())
		{
			pin_data[i].pin->ConsumeInterrupt();
			if (pin_data[i].isr)
			{
				pin_data[i].isr(PinUtils::GetPin(port, i));
			}
		}
	}
}

}
}
