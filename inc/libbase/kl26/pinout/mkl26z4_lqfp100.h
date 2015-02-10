/*
 * mkl26z4_lqfp100.h
 *
 *  Created on: Feb 9, 2015
 *      Author: harrison
 */

#ifndef MKL26Z4_LQFP100_H_
#define MKL26Z4_LQFP100_H_

namespace libbase{
namespace kl26{

class Mkl26z4Lqfp100{
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

	static constexpr Uint GetUartCount()
	{
		return PINOUT_UART_COUNT;
	}

	static Adc::Name GetAdc(const Pin::Name pin);
	static Ftm::Name GetFtm(const Pin::Name pin);
	static Pin::Config::MuxControl GetFtmMux(const Pin::Name pin);
	static Ftm::QdName GetFtmQd(const Pin::Name pin);
	static Pin::Config::MuxControl GetFtmQdMux(const Pin::Name pin);
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
	bool RegPin_(const Dac::Name pin);
	void UnregPin_(const Uint pin);
	void UnregPin_(const Pin::Name pin);
	void UnregPin_(const Adc::Name pin);
	void UnregPin_(const Dac::Name pin);

	std::bitset<kPinCount> m_is_pins_active;
};

}
}

#endif /* MKL26Z4_LQFP100_H_ */
