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

namespace libsc
{

#ifdef LIBSC_USE_ENCODER

#ifdef LIBSC_USE_ENCODER_FTM

namespace
{

#if LIBSC_USE_ENCODER == 1
#define GetQdA(x) FtmUtils::GetFtmModule<LIBSC_ENCODER0_QDA>()

#else
inline FTMn_e GetQdA(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return FtmUtils::GetFtmModule<LIBSC_ENCODER0_QDA>();

	case 1:
		return FtmUtils::GetFtmModule<LIBSC_ENCODER1_QDA>();
	}
}

#endif

}

Encoder::Encoder(const uint8_t id)
		: m_id(id), m_count(0)
{
	if (id >= LIBSC_USE_ENCODER)
	{
		LOG_E("Requested encoder does not exists");
		return;
	}
	FTM_QUAD_Init(GetQdA(m_id));
}

void Encoder::Update()
{
	m_count = FTM_QUAD_get(GetQdA(m_id));
	FTM_QUAD_clean(GetQdA(m_id));
	return;
}

#else
namespace
{

#if LIBSC_USE_ENCODER == 1
#define GetGpio(x) LIBSC_ENCODER0_QDA

#else
inline PTXn_e GetGpio(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_ENCODER0_QDA;

	case 1:
		return LIBSC_ENCODER1_QDA;
	}
}

#endif

volatile int16_t g_count[LIBSC_USE_ENCODER] = {};

void IsrHandler(const PTX_e port, const PTn_e pin)
{
	switch (PTXn(port, pin))
	{
	case LIBSC_ENCODER0_QDA:
		++g_count[0];
		break;

#if LIBSC_USE_ENCODER > 1
	case LIBSC_ENCODER1_QDA:
		++g_count[1];
		break;
#endif
	}
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
	port_init(GetGpio(m_id), IRQ_RISING | ALT1);
	gpio_ddr(GetGpio(m_id), GPI);
	PortIsrManager::GetInstance()->SetIsrHandler(PTX(GetGpio(m_id)),
			PTn(GetGpio(m_id)), IsrHandler);
}

void Encoder::Update()
{
	m_count = g_count[m_id];
	g_count[m_id] = 0;
}

#endif /* LIBSC_USE_ENCODER_FTM */

#else
Encoder::Encoder(const uint8_t) : m_id(0), m_count(0) {}
void Encoder::Update() {}

#endif /* LIBSC_USE_ENCODER */

}
