/*
 * sim.cpp
 * System integration module
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstdint>
#include <cstring>

#include <array>
#include <string>

#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/sim.h"
#include "libutil/endian_utils.h"
#include "libutil/string.h"

using namespace libutil;
using namespace std;

namespace libbase
{
namespace kl26
{

namespace
{

inline void SetClockGateBit(volatile uint32_t &reg, const Uint shift,
		const bool flag)
{
	if (flag)
	{
		SET_BIT(reg, shift);
	}
	else
	{
		CLEAR_BIT(reg, shift);
	}
}

}

uint32_t Sim::GetRamBytes()
{
	if ((SIM->SDID & SIM_SDID_SRAMSIZE_MASK) == 0)
	{
		return 512;
	}
	else
	{
		return 1 << ((SIM->SDID & SIM_SDID_SRAMSIZE_MASK) - 1);
	}
}

Sim::Kinetis Sim::GetKinetisFamily()
{
	switch (SIM->SDID & SIM_SDID_FAMID_MASK)
	{
	default:
		assert(false);
		// no break

	case SIM_SDID_FAMID(0):
		return Kinetis::kKl00;

	case SIM_SDID_FAMID(1):
		return Kinetis::kKl10;

	case SIM_SDID_FAMID(2):
		return Kinetis::kKl20;

	case SIM_SDID_FAMID(3):
		return Kinetis::kKl30;

	case SIM_SDID_FAMID(4):
		return Kinetis::kKl40;
	}
}

array<Byte, 10> Sim::GetUid()
{
	array<Byte, 10> uid;
	uint16_t temp16 = EndianUtils::HostToBe((SIM->UIDMH & SIM_UIDMH_UID_MASK)
			>> SIM_UIDMH_UID_SHIFT);
	memcpy(uid.data(), &temp16, 2);
	uint32_t temp32 = EndianUtils::HostToBe(SIM->UIDML);
	memcpy(uid.data() + 2, &temp32, 4);
	temp32 = EndianUtils::HostToBe(SIM->UIDL);
	memcpy(uid.data() + 6, &temp32, 4);
	return uid;
}

string Sim::GetUidStr()
{
	return String::Format("%4X%8X%8X",
			(SIM->UIDMH & SIM_UIDMH_UID_MASK) >> SIM_UIDMH_UID_SHIFT, SIM->UIDML,
			SIM->UIDL);
}

void Sim::SetEnableClockGate(const ClockGate cg, const bool flag)
{
	switch (cg)
	{
	case ClockGate::kAdc0:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_ADC0_SHIFT, flag);
		break;

	case ClockGate::kCmp:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_CMP_SHIFT, flag);
		break;

	case ClockGate::kDac0:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_DAC0_SHIFT, flag);
		break;

	case ClockGate::kDma:
		SetClockGateBit(SIM->SCGC7, SIM_SCGC7_DMA_SHIFT, flag);
		break;

	case ClockGate::kDmaMux:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_DMAMUX_SHIFT, flag);
		break;

	case ClockGate::kFlash:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_FTF_SHIFT, flag);
		break;

	case ClockGate::kI2c0:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_I2C0_SHIFT, flag);
		break;

	case ClockGate::kI2c1:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_I2C1_SHIFT, flag);
		break;

	case ClockGate::kI2s:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_I2S_SHIFT, flag);
		break;

	case ClockGate::kLptimer:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_LPTMR_SHIFT, flag);
		break;

	case ClockGate::kPit:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_PIT_SHIFT, flag);
		break;

	case ClockGate::kPortA:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_PORTA_SHIFT, flag);
		break;

	case ClockGate::kPortB:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_PORTB_SHIFT, flag);
		break;

	case ClockGate::kPortC:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_PORTC_SHIFT, flag);
		break;

	case ClockGate::kPortD:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_PORTD_SHIFT, flag);
		break;

	case ClockGate::kPortE:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_PORTE_SHIFT, flag);
		break;

	case ClockGate::kRtc:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_RTC_SHIFT, flag);
		break;

	case ClockGate::kSpi0:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_SPI0_SHIFT, flag);
		break;

	case ClockGate::kSpi1:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_SPI1_SHIFT, flag);
		break;

	case ClockGate::kTsi:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_TSI_SHIFT, flag);
		break;

	case ClockGate::kTpm0:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_TPM0_SHIFT, flag);
		break;

	case ClockGate::kTpm1:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_TPM1_SHIFT, flag);
		break;

	case ClockGate::kTpm2:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_TPM2_SHIFT, flag);
		break;

	case ClockGate::kUart0:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_UART0_SHIFT, flag);
		break;

	case ClockGate::kUart1:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_UART1_SHIFT, flag);
		break;

	case ClockGate::kUart2:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_UART2_SHIFT, flag);
		break;

	case ClockGate::kUsbOtg:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_USBOTG_SHIFT, flag);
		break;

	default:
		assert(false);
	}
}

}
}
