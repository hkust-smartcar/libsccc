/*
 * encoder.cpp
 * Encoder
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdio>

#include <misc.h>
#include <MK60_FTM.h>
#include <MK60_gpio.h>
#include <MK60_port.h>
#include <vectors.h>

#include "libsc/com/config.h"
#include "libsc/com/k60/ftm_utils.h"
#include "libsc/com/k60/port_isr_manager.h"

#include "libsc/com/encoder.h"

#define VECTOR_GPIO(x) static_cast<VECTORn_t>(PORTA_VECTORn + PTX(x))
#define IRQ_GPIO(x) static_cast<IRQn_t>(PORTA_IRQn + PTX(x))
#define ISFR(x) PORT_ISFR_REG(PORTX_BASE(x))

namespace libsc
{

#ifdef LIBSC_USE_ENCODER

#ifdef LIBSC_USE_ENCODER_FTM
Encoder::Encoder(const uint8_t id)
		: m_id(id), m_count(0)
{
	if (id >= LIBSC_USE_ENCODER)
	{
		LOG_E("Requested encoder does not exists");
		return;
	}
	FTM_QUAD_Init(FtmUtils::GetFtmModule<LIBSC_ENCODER0_OUT>());
}

uint32_t Encoder::GetCount()
{
	m_count += FTM_QUAD_get(FtmUtils::GetFtmModule<LIBSC_ENCODER0_OUT>());
	FTM_QUAD_clean(FtmUtils::GetFtmModule<LIBSC_ENCODER0_OUT>());
	return m_count;
}

#else
namespace
{

volatile uint32_t g_count = 0;

__ISR void IsrHandler()
{
	++g_count;
}

}

Encoder::Encoder(const uint8_t id)
		: m_id(id), m_count(0)
{
	if (id >= LIBSC_USE_ENCODER)
	{
		LOG_E("Requested encoder does not exists");
		return;
	}
	// TODO Support more than one encoder
	port_init(LIBSC_ENCODER0, IRQ_RISING | ALT1);
	gpio_ddr(LIBSC_ENCODER0, GPI);
	PortIsrManager::GetInstance()->SetIsrHandler(
			static_cast<PTX_e>(PTX(LIBSC_ENCODER0)),
			static_cast<PTn_e>(PTn(LIBSC_ENCODER0)), IsrHandler);
}

uint32_t Encoder::GetCount()
{
	return g_count;
}

#endif /* LIBSC_USE_ENCODER_FTM */

#else
Encoder::Encoder(const uint8_t) : m_id(0) {}
uint32_t Encoder::GetCount() { return 0; }

#endif /* LIBSC_USE_ENCODER */

}
