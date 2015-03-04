/*
 * mkl26z4_lqfp100.h
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <bitset>

#include "libbase/kl26/adc.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout/mkl26z4_lqfp100_macros.h"
#include "libbase/kl26/tpm.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace kl26
{

class Mkl26z4Lqfp100
{
public:
	static bool RegPin(const Uint pin)
	{
		return Get().RegPin_(pin);
	}

	static bool RegPin(const Pin::Name pin)
	{
		return Get().RegPin_(pin);
	}

	static bool RegPin(const Adc::Name pin)
	{
		return Get().RegPin_(pin);
	}

	static void UnregPin(const Uint pin)
	{
		Get().UnregPin_(pin);
	}

	static void UnregPin(const Pin::Name pin)
	{
		Get().UnregPin_(pin);
	}

	static void UnregPin(const Adc::Name pin)
	{
		Get().UnregPin_(pin);
	}

	static constexpr Uint GetAdcCount()
	{
		return PINOUT_ADC_COUNT;
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

	static constexpr Uint GetTpmCount()
	{
		return PINOUT_TPM_COUNT;
	}

	static constexpr Uint GetTpmChannelCount()
	{
		return PINOUT_TPM_CHANNEL_COUNT;
	}

	static constexpr Uint GetUartCount()
	{
		return PINOUT_UART_COUNT;
	}

	static Adc::Name GetAdc(const Pin::Name pin);
	static Tpm::Name GetTpm(const Pin::Name pin);
	static Pin::Config::MuxControl GetTpmMux(const Pin::Name pin);
	static Tpm::ClkinName GetTpmClkin(const Pin::Name pin);
	static Pin::Config::MuxControl GetTpmClkinMux(const Pin::Name pin);

private:
	static constexpr Uint kPinCount = PINOUT_PIN_COUNT;

	Mkl26z4Lqfp100();

	static Mkl26z4Lqfp100& Get()
	{
		static Mkl26z4Lqfp100 inst;
		return inst;
	}

	bool RegPin_(const Uint pin);
	bool RegPin_(const Pin::Name pin);
	bool RegPin_(const Adc::Name pin);
	void UnregPin_(const Uint pin);
	void UnregPin_(const Pin::Name pin);
	void UnregPin_(const Adc::Name pin);

	std::bitset<kPinCount> m_is_pins_active;
};

}
}
