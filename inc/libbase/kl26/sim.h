/*
 * sim.h
 * System integration module
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <array>
#include <string>

#include "libbase/kl26/misc_utils.h"

namespace libbase
{
namespace kl26
{

class Sim
{
public:
	enum struct Kinetis
	{
		kK10,
		kK20,
		kK30,
		kK40,
		kK50K52,
		kK51K53,
		kK60
	};

#ifdef MKL26Z4
	enum struct ClockGate
	{
		kAdc0,
//		kAdc1,
		kCmp,
		kCmt,
//		kCrc,
//		kDac0,
//		kDac1,
		kDma,
		kDmaMux,
//		kEthernet,
//		kExtWatchdog,
//		kFlexBus,
//		kFlexCan0,
//		kFlexCan1,
		kFlash,
//		kFtm0,
//		kFtm1,
//		kFtm2,
		kI2c0,
		kI2c1,
		kI2s,
//		kLlwu,
//		kLptimer,
//		kMpu,
//		kPdb,
		kPit,
		kPortA,
		kPortB,
		kPortC,
		kPortD,
		kPortE,
		kRegFile,
//		kRng,
		kRtc,
//		kSdhc,
		kSpi0,
		kSpi1,
//		kSpi2,
		kTsi,
		kUart0,
		kUart1,
		kUart2,
//		kUart3,
//		kUart4,
//		kUart5,
//		kUsbDcd,
		kUsbOtg,
//		kVref,
	};

#endif

	static uint32_t GetRamBytes();
	static Uint GetRamKBytes()
	{
		return GetRamBytes() >> 10;
	}

	static Kinetis GetKinetisFamily();
	static std::array<Byte, 16> GetUid();
	static std::string GetUidStr();

	static void SetEnableClockGate(const ClockGate cg, const bool flag);
};

}
}
