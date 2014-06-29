/*
 * pin.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_PIN_H_
#define LIBBASE_K60_PIN_H_

#include <cstdint>

#include <bitset>

namespace libbase
{
namespace k60
{

struct PinConfig
{
	enum Name
	{
		PTA0 = 0, PTA1, PTA2, PTA3, PTA4, PTA5, PTA6, PTA7, PTA8, PTA9,
		PTA10, PTA11, PTA12, PTA13, PTA14, PTA15, PTA16, PTA17, PTA18, PTA19,
		PTA24 = 24, PTA25, PTA26, PTA27, PTA28, PTA29,

		PTB0 = 32, PTB1, PTB2, PTB3, PTB4, PTB5, PTB6, PTB7, PTB8, PTB9,
		PTB10, PTB11, PTB16 = 48, PTB17, PTB18, PTB19,
		PTB20, PTB21, PTB22, PTB23,

		PTC0 = 64, PTC1, PTC2, PTC3, PTC4, PTC5, PTC6, PTC7, PTC8, PTC9,
		PTC10, PTC11, PTC12, PTC13, PTC14, PTC15, PTC16, PTC17, PTC18, PTC19,

		PTD0 = 96, PTD1, PTD2, PTD3, PTD4, PTD5, PTD6, PTD7, PTD8, PTD9,
		PTD10, PTD11, PTD12, PTD13, PTD14, PTD15,

		PTE0 = 128, PTE1, PTE2, PTE3, PTE4, PTE5, PTE6, PTE7, PTE8, PTE9,
		PTE10, PTE11, PTE12,
		PTE24 = 152, PTE25, PTE26, PTE27, PTE28
	};

	enum struct MuxControl
	{
		DISABLE,
		GPIO,
		ALT2, ALT3, ALT4, ALT5, ALT6, ALT7,
	};

	enum struct Interrupt
	{
		DISABLE,
		DMA_RISING, DMA_FALLING, DMA_BOTH,
		RISING, FALLING, BOTH,
		LOW, HIGH
	};

	enum ConfigBit
	{
		HIGH_DRIVE_STRENGTH = 0,
		OPEN_DRAIN,
		// Disable the passive input filter when high speed interfaces of more
		// than 2 MHz are supported on the pin
		PASSIVE_FILTER,
		SLOW_SLEW_RATE,
		// Enable the internal pull-up or pull-down resistor
		PULL_ENABLE,
		PULL_UP,

		SIZE,
	};

	static constexpr int PORT_COUNT = 5;
	static constexpr int PORT_PIN_COUNT = 32;

	Name pin;
	MuxControl mux = MuxControl::DISABLE;
	Interrupt interrupt = Interrupt::DISABLE;
	std::bitset<ConfigBit::SIZE> config = 0x0;
};

class Pin
{
public:
	explicit Pin(const PinConfig &config);
	Pin(Pin &&rhs);
	explicit Pin(nullptr_t);
	~Pin();

	Pin& operator=(Pin &&rhs);

	PinConfig::Name GetName() const
	{
		return m_name;
	}

	void SetInterrupt(const PinConfig::Interrupt config);
	bool IsInterruptRequested() const;
	void ConsumeInterrupt();

private:
	PinConfig::Name m_name;
};

}
}

#endif /* LIBBASE_K60_PIN_H_ */
