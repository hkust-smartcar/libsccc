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
/*
	SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;
	SIM_SCGC5 |=   SIM_SCGC5_PORTB_MASK;
    PORTB_PCR18 =(0|PORT_PCR_MUX(6));        // set PTB18,PTB19 be quadrature mode
	PORTB_PCR19 =(0|PORT_PCR_MUX(6));
	FTM2_SC = 0x00;                           // off counter, interrupt
    FTM2_CNT = 0x0000;                         // initialise counter
	FTM_CNTIN_REG(FTM2_BASE_PTR) = 0x0000;
	FTM_MODE_REG(FTM2_BASE_PTR) |= (FTM_MODE_WPDIS_MASK|FTM_MODE_FTMEN_MASK);   // WPDIS =1, FTMEN=1
	FTM_MOD_REG(FTM2_BASE_PTR) = 0xFFFF;       // maximum count = 65535
	FTM2_QDCTRL = 0x0F;
*/
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
	PortIsrManager::GetInstance()->AddIsrHandler(
			static_cast<PTX_e>(PTX(LIBSC_ENCODER0)),
			static_cast<PTn_e>(PTn(LIBSC_ENCODER0)), IsrHandler);
}

uint32_t Encoder::GetCount()
{
	return g_count;
}

#endif /* LIBSC_USE_ENCODER_FTM */

}
