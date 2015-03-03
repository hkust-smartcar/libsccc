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
		kKl00,
		kKl10,
		kKl20,
		kKl30,
		kKl40,
	};

	enum struct ClockGate
	{
		kAdc0,
		kCmp,
		kDac0,
		kDma,
		kDmaMux,
		kFlash,
		kI2c0,
		kI2c1,
		kI2s,
		kLptimer,
		kPit,
		kPortA,
		kPortB,
		kPortC,
		kPortD,
		kPortE,
		kRtc,
		kSpi0,
		kSpi1,
		kTsi,
		kTpm0,
		kTpm1,
		kTpm2,
		kUart0,
		kUart1,
		kUart2,
		kUsbOtg,
	};

	static uint32_t GetRamBytes();
	static Uint GetRamKBytes()
	{
		return GetRamBytes() >> 10;
	}

	static Kinetis GetKinetisFamily();
	static std::array<Byte, 10> GetUid();
	static std::string GetUidStr();

	static void SetEnableClockGate(const ClockGate cg, const bool flag);
	static uint8_t GetCoreClockDivider();
	static uint8_t GetBusClockDivider();
	static uint8_t GetFlashClockDivider();
};

}
}
