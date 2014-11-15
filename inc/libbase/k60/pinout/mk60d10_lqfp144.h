/*
 * mk60d10_lqfp144.h
 * Pinout for:
 * MK60DN256VLQ10
 * MK60DX256VLQ10
 * MK60DN512VLQ10
 * MK60DN256VMD10
 * MK60DX256VMD10
 * MK60DN512VMD10
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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

class Mk60d10Lqfp144
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

	static constexpr Uint GetAdcCount()
	{
		return PINOUT_ADC_COUNT;
	}

	static constexpr Uint GetFtmCount()
	{
		return PINOUT_FTM_COUNT;
	}

	static constexpr Uint GetFtmChannelCount()
	{
		return PINOUT_FTM_CHANNEL_COUNT;
	}

	static constexpr Uint GetPinCount()
	{
		return kPinCount;
	}

	static constexpr Uint GetPortCount()
	{
		return PINOUT_PORT_COUNT;
	}

	static constexpr Uint GetPortPinCount()
	{
		return PINOUT_PORT_PIN_COUNT;
	}

	static constexpr Uint GetSpiCount()
	{
		return PINOUT_SPI_COUNT;
	}

	static constexpr Uint GetUartCount()
	{
		return PINOUT_UART_COUNT;
	}

	static Adc::Name GetAdc(const Pin::Name pin);

private:
	static constexpr Uint kPinCount = PINOUT_PIN_COUNT;

	Mk60d10Lqfp144();

	static Mk60d10Lqfp144* GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Mk60d10Lqfp144;
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

	static Mk60d10Lqfp144 *m_instance;
};

}
}
