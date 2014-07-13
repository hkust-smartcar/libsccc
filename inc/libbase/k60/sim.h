/*
 * sim.h
 * System integration module
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_SIM_H_
#define LIBBASE_K60_SIM_H_

#include <cstdint>

#include <array>
#include <string>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
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

	enum struct ClockGate
	{
		kAdc0,
		kAdc1,
		kCmp,
		kCmt,
		kCrc,
		kDac0,
		kDac1,
		kDma,
		kDmaMux,
		kEthernet,
		kExtWatchdog,
		kFlexBus,
		kFlexCan0,
		kFlexCan1,
		kFlash,
		kFtm0,
		kFtm1,
		kFtm2,
		kI2c0,
		kI2c1,
		kI2s,
		kLlwu,
		kLptimer,
		kMpu,
		kPdb,
		kPit,
		kPortA,
		kPortB,
		kPortC,
		kPortD,
		kPortE,
		kRnga,
		kRtc,
		kSdhc,
		kSpi0,
		kSpi1,
		kSpi2,
		kTsi,
		kUart0,
		kUart1,
		kUart2,
		kUart3,
		kUart4,
		kUart5,
		kUsbDcd,
		kUsbOtg,
		kVref,
	};

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

#endif /* LIBBASE_K60_SIM_H_ */
