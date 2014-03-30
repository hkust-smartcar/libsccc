/*
 * port_isr_manager.h
 * Manage ISR for ports
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_PORT_ISR_MANAGER_H_
#define LIBSC_PORT_ISR_MANAGER_H_

#include <MK60_gpio.h>
#include <vectors.h>

namespace libsc
{

class PortIsrManager
{
public:
	static PortIsrManager* GetInstance();

	void SetIsrHandler(const PTX_e port, const PTn_e pin, tIsrFunc fn);

private:
	static constexpr int PORT_COUNT = 5;
	static constexpr int PIN_COUNT = 32;

	PortIsrManager();
	~PortIsrManager();

	void InitPort(const PTX_e port);

	template<PTX_e port>
	static __ISR void IsrHandler();

	tIsrFunc* m_handlers[PORT_COUNT];
	bool m_is_enabled[PORT_COUNT];

	static PortIsrManager *m_instance;
};

}

#endif /* LIBSC_PORT_ISR_MANAGER_H_ */
