/*
 * pin.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <bitset>

namespace libbase
{
namespace k60
{

class Pin
{
public:
	enum struct Name
	{
		kPta0 = 0,
		kPta1,
		kPta2,
		kPta3,
		kPta4,
		kPta5,
		kPta6,
		kPta7,
		kPta8,
		kPta9,
		kPta10,
		kPta11,
		kPta12,
		kPta13,
		kPta14,
		kPta15,
		kPta16,
		kPta17,
		kPta18,
		kPta19,
		kPta20,
		kPta21,
		kPta22,
		kPta23,
		kPta24,
		kPta25,
		kPta26,
		kPta27,
		kPta28,
		kPta29,
		kPta30,
		kPta31,

		kPtb0 = 32,
		kPtb1,
		kPtb2,
		kPtb3,
		kPtb4,
		kPtb5,
		kPtb6,
		kPtb7,
		kPtb8,
		kPtb9,
		kPtb10,
		kPtb11,
		kPtb12,
		kPtb13,
		kPtb14,
		kPtb15,
		kPtb16,
		kPtb17,
		kPtb18,
		kPtb19,
		kPtb20,
		kPtb21,
		kPtb22,
		kPtb23,
		kPtb24,
		kPtb25,
		kPtb26,
		kPtb27,
		kPtb28,
		kPtb29,
		kPtb30,
		kPtb31,

		kPtc0 = 64,
		kPtc1,
		kPtc2,
		kPtc3,
		kPtc4,
		kPtc5,
		kPtc6,
		kPtc7,
		kPtc8,
		kPtc9,
		kPtc10,
		kPtc11,
		kPtc12,
		kPtc13,
		kPtc14,
		kPtc15,
		kPtc16,
		kPtc17,
		kPtc18,
		kPtc19,
		kPtc20,
		kPtc21,
		kPtc22,
		kPtc23,
		kPtc24,
		kPtc25,
		kPtc26,
		kPtc27,
		kPtc28,
		kPtc29,
		kPtc30,
		kPtc31,

		kPtd0 = 96,
		kPtd1,
		kPtd2,
		kPtd3,
		kPtd4,
		kPtd5,
		kPtd6,
		kPtd7,
		kPtd8,
		kPtd9,
		kPtd10,
		kPtd11,
		kPtd12,
		kPtd13,
		kPtd14,
		kPtd15,
		kPtd16,
		kPtd17,
		kPtd18,
		kPtd19,
		kPtd20,
		kPtd21,
		kPtd22,
		kPtd23,
		kPtd24,
		kPtd25,
		kPtd26,
		kPtd27,
		kPtd28,
		kPtd29,
		kPtd30,
		kPtd31,

		kPte0 = 128,
		kPte1,
		kPte2,
		kPte3,
		kPte4,
		kPte5,
		kPte6,
		kPte7,
		kPte8,
		kPte9,
		kPte10,
		kPte11,
		kPte12,
		kPte13,
		kPte14,
		kPte15,
		kPte16,
		kPte17,
		kPte18,
		kPte19,
		kPte20,
		kPte21,
		kPte22,
		kPte23,
		kPte24,
		kPte25,
		kPte26,
		kPte27,
		kPte28,
		kPte29,
		kPte30,
		kPte31,

		kDisable = 160
	};

	struct Config
	{
		enum struct MuxControl
		{
			kAnalog,
			kGpio,
			kAlt2,
			kAlt3,
			kAlt4,
			kAlt5,
			kAlt6,
			kAlt7
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
			/**
			 * Select between high/low drive strength, effective only with
			 * output pin
			 */
			kHighDriveStrength = 0,
			/// Effective only with input pin
			kOpenDrain,
			/**
			 * Enable a low pass filter (10 MHz to 30 MHz bandwidth) on the pin.
			 * Disable when supporting high speed interfaces (> 2MHz). Effective
			 * only with input pin
			 */
			kPassiveFilter,
			/// Effective only with output pin
			kSlowSlewRate,
			/**
			 * Enable the internal pull-up or pull-down resistor, depending on
			 * ConfigBit::kPullUp. Effective only with input pin
			 */
			kPullEnable,
			kPullUp,

			kSize
		};

		Name pin;
		MuxControl mux;
		Interrupt interrupt = Interrupt::kDisable;
		std::bitset<ConfigBit::kSize> config = 0x0;
	};

	explicit Pin(const Config &config);
	explicit Pin(nullptr_t);
	Pin()
			: Pin(nullptr)
	{}
	Pin(const Pin&) = delete;
	Pin(Pin &&rhs);
	~Pin();

	Pin& operator=(const Pin&) = delete;
	Pin& operator=(Pin &&rhs);
	operator bool() const
	{
		return (m_name != Name::kDisable);
	}

	Name GetName() const
	{
		return m_name;
	}

	void SetInterrupt(const Config::Interrupt config);
	bool IsInterruptRequested() const;
	void ConsumeInterrupt();

	static bool IsInterruptRequested(const Pin::Name pin);
	static void ConsumeInterrupt(const Pin::Name pin);

private:
	void Uninit();

	Name m_name;
};

}
}
