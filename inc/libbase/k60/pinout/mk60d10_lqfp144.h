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
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <bitset>

#include "libbase/k60/adc.h"
#include "libbase/k60/dac.h"
#include "libbase/k60/dma_mux.h"
#include "libbase/k60/ftm.h"
#include "libbase/k60/i2c.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout/mk60d10_lqfp144_macros.h"
#include "libbase/k60/spi.h"
#include "libbase/k60/uart.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace k60
{

class Mk60d10Lqfp144
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

	static bool RegPin(const Dac::Name pin)
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

	static void UnregPin(const Dac::Name pin)
	{
		Get().UnregPin_(pin);
	}

	static constexpr Uint GetAdcCount()
	{
		return PINOUT_ADC_COUNT;
	}

	static constexpr Uint GetDmaChCount()
	{
		return PINOUT_DMA_CH_COUNT;
	}

	static constexpr Uint GetDmaMuxCount()
	{
		return PINOUT_DMA_MUX_COUNT;
	}

	static constexpr Uint GetFtmCount()
	{
		return PINOUT_FTM_COUNT;
	}

	static constexpr Uint GetFtmChannelCount()
	{
		return PINOUT_FTM_CHANNEL_COUNT;
	}

	static constexpr Uint GetI2cCount()
	{
		return PINOUT_I2C_COUNT;
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
	static Ftm::Name GetFtm(const Pin::Name pin);
	static Pin::Config::MuxControl GetFtmMux(const Pin::Name pin);
	static Ftm::QdName GetFtmQd(const Pin::Name pin);
	static Pin::Config::MuxControl GetFtmQdMux(const Pin::Name pin);
	static I2c::Name GetI2c(const Pin::Name pin);
	static Pin::Config::MuxControl GetI2cMux(const Pin::Name pin);
	static Spi::SinName GetSpiSin(const Pin::Name pin);
	static Pin::Config::MuxControl GetSpiSinMux(const Pin::Name pin);
	static Spi::SoutName GetSpiSout(const Pin::Name pin);
	static Pin::Config::MuxControl GetSpiSoutMux(const Pin::Name pin);
	static Spi::SckName GetSpiSck(const Pin::Name pin);
	static Pin::Config::MuxControl GetSpiSckMux(const Pin::Name pin);
	static Spi::PcsName GetSpiPcs(const Pin::Name pin);
	static Pin::Config::MuxControl GetSpiPcsMux(const Pin::Name pin);
	/**
	 * Return the source number for the specific module @a mux, or -1 if @a src
	 * doesn't exist in the module
	 *
	 * @param src
	 * @param mux
	 * @return
	 */
	static uint8_t GetDmaMuxSource(const DmaMux::Source src, const Uint mux);
	static Uart::Name GetUart(const Pin::Name pin);
	static Pin::Config::MuxControl GetUartMux(const Pin::Name pin);

private:
	static constexpr Uint kPinCount = PINOUT_PIN_COUNT;

	Mk60d10Lqfp144();

	static Mk60d10Lqfp144& Get()
	{
		static Mk60d10Lqfp144 inst;
		return inst;
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
};

}
}
