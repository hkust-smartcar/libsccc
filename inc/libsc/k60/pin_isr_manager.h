/*
 * pin_isr_manager.h
 * Manage ISR for pins
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_PIN_ISR_MANAGER_H_
#define LIBSC_K60_PIN_ISR_MANAGER_H_

#include <vectors.h>

#include <bitset>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libsc
{
namespace k60
{

class PinIsrManager
{
public:
	typedef void (*OnPinIrqListener)(const libbase::k60::PinConfig::Name pin);

	static PinIsrManager* GetInstance();

	void SetPinIsr(libbase::k60::Pin *pin, OnPinIrqListener isr);

private:
	struct PinData
	{
		libbase::k60::Pin *pin = nullptr;
		OnPinIrqListener isr = nullptr;
	};

	PinIsrManager();
	~PinIsrManager();

	void InitPort(const Uint port);

	template<Uint port>
	static __ISR void PortIrqHandler();

	PinData* m_pin_data[libbase::k60::PinConfig::PORT_COUNT];
	bool m_is_enable[libbase::k60::PinConfig::PORT_COUNT];

	static PinIsrManager *m_instance;
};

}
}

#endif /* LIBSC_K60_PIN_ISR_MANAGER_H_ */
