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
		PTE24 = 152, PTE25, PTE26, PTE27, PTE28,

		DISABLE = 160
	};

	enum struct MuxControl
	{
		kDisable,
		kGpio,
		kAlt2, kAlt3, kAlt4, kAlt5, kAlt6, kAlt7
	};

	enum struct Interrupt
	{
		kDisable,

		kDmaRising,
		kDmaFalling,
		kDmaBoth,

		kRising,
		kFalling,
		kBoth,

		kLow,
		kHigh
	};

	enum ConfigBit
	{
		kHighDriveStrength = 0,
		kOpenDrain,
		// Disable the passive input filter when high speed interfaces of more
		// than 2 MHz are supported on the pin
		kPassiveFilter,
		kSlowSlewRate,
		// Enable the internal pull-up or pull-down resistor
		kPullEnable,
		kPullUp,

		kSize
	};

	static constexpr int PORT_COUNT = 5;
	static constexpr int PORT_PIN_COUNT = 32;

	Name pin = Name::DISABLE;
	MuxControl mux = MuxControl::kDisable;
	Interrupt interrupt = Interrupt::kDisable;
	std::bitset<ConfigBit::kSize> config = 0x0;
};

class Pin
{
public:
	explicit Pin(const PinConfig &config);
	Pin(const Pin&) = delete;
	Pin(Pin &&rhs);
	explicit Pin(nullptr_t);
	~Pin();

	Pin& operator=(const Pin&) = delete;
	Pin& operator=(Pin &&rhs);

	PinConfig::Name GetName() const
	{
		return m_name;
	}

	void SetInterrupt(const PinConfig::Interrupt config);
	bool IsInterruptRequested() const;
	void ConsumeInterrupt();

private:
	void Uninit();

	PinConfig::Name m_name;
};

}
}

#endif /* LIBBASE_K60_PIN_H_ */
