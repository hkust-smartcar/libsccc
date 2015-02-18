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
#include "libutil/string.h"

using libutil::String;
using namespace std;

#ifdef MKL26Z4
	#define _SIM_SCGC6_DMAMUX0_SHIFT SIM_SCGC6_DMAMUX_SHIFT
	#define _SIM_SCGC4_SPI0_SHIFT SIM_SCGC4_SPI0_SHIFT
	#define _SIM_SCGC4_SPI1_SHIFT SIM_SCGC4_SPI1_SHIFT
	#define _SIM_SCGC4_I2C0_SHIFT SIM_SCGC4_I2C0_SHIFT
	#define _SIM_SCGC4_I2C1_SHIFT SIM_SCGC4_I2C1_SHIFT
#endif

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
	/*switch (SIM->SOPT1 & SIM_SOPT1_RAMSIZE_MASK)
	{
	default:
		assert(false);
		// no break

	case SIM_SOPT1_RAMSIZE(0x1):
		return 8 << 10;

	case SIM_SOPT1_RAMSIZE(0x3):
		return 16 << 10;

	case SIM_SOPT1_RAMSIZE(0x5):
		return 32 << 10;

	case SIM_SOPT1_RAMSIZE(0x7):
		return 64 << 10;

	case SIM_SOPT1_RAMSIZE(0x9):
		return 128 << 10;
	}*/
}

Sim::Kinetis Sim::GetKinetisFamily()
{
	switch (SIM->SDID & SIM_SDID_FAMID_MASK)
	{
	default:
		assert(false);
		// no break

	case SIM_SDID_FAMID(0):
		return Kinetis::kK10;

	case SIM_SDID_FAMID(1):
		return Kinetis::kK20;

	case SIM_SDID_FAMID(2):
		return Kinetis::kK30;

	case SIM_SDID_FAMID(3):
		return Kinetis::kK40;

	case SIM_SDID_FAMID(4):
		return Kinetis::kK60;

	case SIM_SDID_FAMID(6):
		return Kinetis::kK50K52;

	case SIM_SDID_FAMID(7):
		return Kinetis::kK51K53;
	}
}

array<Byte, 16> Sim::GetUid()
{
	array<Byte, 16> uid;
	/*uint32_t temp = htobe32(SIM->UIDH);
	memcpy(uid.data(), &temp, 4);
	temp = htobe32(SIM->UIDMH);
	memcpy(uid.data() + 4, &temp, 4);
	temp = htobe32(SIM->UIDML);
	memcpy(uid.data() + 8, &temp, 4);
	temp = htobe32(SIM->UIDL);
	memcpy(uid.data() + 12, &temp, 4);*/
	return uid;
}

string Sim::GetUidStr()
{
	/*return String::Format("%8X%8X%8X%8X", SIM->UIDH, SIM->UIDMH, SIM->UIDML,
			SIM->UIDL);*/
}

void Sim::SetEnableClockGate(const ClockGate cg, const bool flag)
{
	switch (cg)
	{
	case ClockGate::kAdc0:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_ADC0_SHIFT, flag);
		break;

/*	case ClockGate::kAdc1:
		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_ADC1_SHIFT, flag);
		break;*/

	case ClockGate::kCmp:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_CMP_SHIFT, flag);
		break;

	/*case ClockGate::kCmt:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_CMT_SHIFT, flag);
		break;

	case ClockGate::kCrc:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_CRC_SHIFT, flag);
		break;

	case ClockGate::kDac0:
		SetClockGateBit(SIM->SCGC2, SIM_SCGC2_DAC0_SHIFT, flag);
		break;

	case ClockGate::kDac1:
		SetClockGateBit(SIM->SCGC2, SIM_SCGC2_DAC1_SHIFT, flag);
		break;*/

	case ClockGate::kDma:
		SetClockGateBit(SIM->SCGC7, SIM_SCGC7_DMA_SHIFT, flag);
		break;


	case ClockGate::kDmaMux:
		SetClockGateBit(SIM->SCGC6, _SIM_SCGC6_DMAMUX0_SHIFT, flag);
		break;

//	case ClockGate::kEthernet:
//		SetClockGateBit(SIM->SCGC2, SIM_SCGC2_ENET_SHIFT, flag);
//		break;

//	case ClockGate::kExtWatchdog:
//		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_EWM_SHIFT, flag);
//		break;
//
//	case ClockGate::kFlexBus:
//		SetClockGateBit(SIM->SCGC7, SIM_SCGC7_FLEXBUS_SHIFT, flag);
//		break;
//
//	case ClockGate::kFlexCan0:
//		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_FLEXCAN0_SHIFT, flag);
//		break;
//
//	case ClockGate::kFlexCan1:
//		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_FLEXCAN1_SHIFT, flag);
//		break;

#if MK60DZ10 || MK60D10
	case ClockGate::kFlash:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_FTFL_SHIFT, flag);
		break;
#endif

//	case ClockGate::kFtm0:
//		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_FTM0_SHIFT, flag);
//		break;
//
//	case ClockGate::kFtm1:
//		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_FTM1_SHIFT, flag);
//		break;
//
//	case ClockGate::kFtm2:
//		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_FTM2_SHIFT, flag);
//		break;

//#if MK60F15
//	case ClockGate::kFtm3:
//		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_FTM3_SHIFT, flag);
//		break;
//#endif


	case ClockGate::kI2c0:
		SetClockGateBit(SIM->SCGC4, _SIM_SCGC4_I2C0_SHIFT, flag);
		break;


	case ClockGate::kI2c1:
		SetClockGateBit(SIM->SCGC4, _SIM_SCGC4_I2C1_SHIFT, flag);
		break;

#if MK60DZ10 || MK60D10
	case ClockGate::kI2s:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_I2S_SHIFT, flag);
		break;
#endif

/*	case ClockGate::kLlwu:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_LLWU_SHIFT, flag);
		break;
*/

//	case ClockGate::kLptimer:
//		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_LPTIMER_SHIFT, flag);
//		break;

/*
	case ClockGate::kMpu:
		SetClockGateBit(SIM->SCGC7, SIM_SCGC7_MPU_SHIFT, flag);
		break;*/

#if MK60F15
	case ClockGate::kNfc:
		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_NFC_SHIFT, flag);
		break;

	case ClockGate::kOsc1:
		SetClockGateBit(SIM->SCGC1, SIM_SCGC1_OSC1_SHIFT, flag);
		break;
#endif

/*	case ClockGate::kPdb:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_PDB_SHIFT, flag);
		break;*/

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

/*	case ClockGate::kRegFile:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_REGFILE_SHIFT, flag);
		break;

	case ClockGate::kRng:
#ifdef MK60DZ10
		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_RNGB_SHIFT, flag);
#else
		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_RNGA_SHIFT, flag);
#endif
		break;*/

	case ClockGate::kRtc:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_RTC_SHIFT, flag);
		break;

#if MK60F15
	case ClockGate::kSai0:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_SAI0_SHIFT, flag);
		break;

	case ClockGate::kSai1:
		SetClockGateBit(SIM->SCGC3, SIM_SCGC3_SAI1_SHIFT, flag);
		break;
#endif

	case ClockGate::kSpi0:
		SetClockGateBit(SIM->SCGC6, _SIM_SCGC4_SPI0_SHIFT, flag);
		break;


	case ClockGate::kSpi1:
		SetClockGateBit(SIM->SCGC6, _SIM_SCGC4_SPI1_SHIFT, flag);
		break;

/*#if MK60DZ10 || MK60D10
	case ClockGate::kSpi2:
#elif MK60F15
	case ClockGate::kDspi2:
#endif
		SetClockGateBit(SIM->SCGC3, _SIM_SCGC3_SPI2_SHIFT, flag);
		break;*/

	case ClockGate::kTsi:
		SetClockGateBit(SIM->SCGC5, SIM_SCGC5_TSI_SHIFT, flag);
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

/*	case ClockGate::kUart3:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_UART3_SHIFT, flag);
		break;

	case ClockGate::kUart4:
		SetClockGateBit(SIM->SCGC1, SIM_SCGC1_UART4_SHIFT, flag);
		break;

	case ClockGate::kUart5:
		SetClockGateBit(SIM->SCGC1, SIM_SCGC1_UART5_SHIFT, flag);
		break;

	case ClockGate::kUsbDcd:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_USBDCD_SHIFT, flag);
		break;*/

#if MK60F15
	case ClockGate::kUsbFs:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_USBFS_SHIFT, flag);
		break;

	case ClockGate::kUsbHs:
		SetClockGateBit(SIM->SCGC6, SIM_SCGC6_USBHS_SHIFT, flag);
		break;
#endif

#if MK60DZ10 || MK60D10
	case ClockGate::kUsbOtg:
		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_USBOTG_SHIFT, flag);
		break;
#endif

//	case ClockGate::kVref:
//		SetClockGateBit(SIM->SCGC4, SIM_SCGC4_VREF_SHIFT, flag);
//		break;

	default:
		assert(false);
	}
}

}
}
