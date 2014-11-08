/*
 * mk60f15_lqfp144.h
 * Pinout for:
 * MK60FX512VLQ15
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <bitset>

#include "libbase/k60/adc.h"
#include "libbase/k60/dac.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class Mk60f15Lqfp144
{
public:
	static bool RegPin(const Uint pin)
	{
		return GetInstance()->RegPin_(pin);
	}

	static bool RegPin(const Pin::Name pin)
	{
		return GetInstance()->RegPin_(pin);
	}

	static bool RegPin(const Adc::Name pin)
	{
		return GetInstance()->RegPin_(pin);
	}

	static bool RegPin(const Dac::Name pin)
	{
		return GetInstance()->RegPin_(pin);
	}

	static void UnregPin(const Uint pin)
	{
		GetInstance()->UnregPin_(pin);
	}

	static void UnregPin(const Pin::Name pin)
	{
		GetInstance()->UnregPin_(pin);
	}

	static void UnregPin(const Adc::Name pin)
	{
		GetInstance()->UnregPin_(pin);
	}

	static void UnregPin(const Dac::Name pin)
	{
		GetInstance()->UnregPin_(pin);
	}

	static constexpr Uint GetFtmCount()
	{
		return 4;
	}

	static constexpr Uint GetFtmChannelCount()
	{
		return 8;
	}

	static constexpr Uint GetPinCount()
	{
		return kPinCount;
	}

	static constexpr Uint GetPortCount()
	{
		return 5;
	}

	static constexpr Uint GetPortPinCount()
	{
		return 32;
	}

	static constexpr Uint GetSpiCount()
	{
		return 3;
	}

	static constexpr Uint GetUartCount()
	{
		return 6;
	}

private:
	static constexpr Uint kPinCount = 144;

	Mk60f15Lqfp144();

	static Mk60f15Lqfp144* GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Mk60f15Lqfp144;
		}
		return m_instance;
	}

	bool RegPin_(const Uint pin);
	bool RegPin_(const Pin::Name pin);
	bool RegPin_(const Adc::Name pin);
	bool RegPin_(const Dac::Name pin);
	void UnregPin_(const Uint pin);
	void UnregPin_(const Pin::Name pin);
	void UnregPin_(const Adc::Name pin);
	void UnregPin_(const Dac::Name pin);

	std::bitset<kPinCount> m_is_pins_active;

	static Mk60f15Lqfp144 *m_instance;
};

}
}
