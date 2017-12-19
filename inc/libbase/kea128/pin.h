/*
 * pin.h
 *
 * Author: LeeChunHei
 * Copyright (c) 2017-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <bitset>

namespace libbase {
namespace kea128 {

class Pin {
public:
	enum struct Name {
		kPta0, kPta1, kPta2, kPta3, kPta4, kPta5, kPta6, kPta7,

		kPtb0, kPtb1, kPtb2, kPtb3, kPtb4, kPtb5, kPtb6, kPtb7,

		kPtc0, kPtc1, kPtc2, kPtc3, kPtc4, kPtc5, kPtc6, kPtc7,

		kPtd0, kPtd1, kPtd2, kPtd3, kPtd4, kPtd5, kPtd6, kPtd7,

		kPte0, kPte1, kPte2, kPte3, kPte4, kPte5, kPte6, kPte7,

		kPtf0, kPtf1, kPtf2, kPtf3, kPtf4, kPtf5, kPtf6, kPtf7,

		kPtg0, kPtg1, kPtg2, kPtg3, kPtg4, kPtg5, kPtg6, kPtg7,

		kPth0, kPth1, kPth2, kPth3, kPth4, kPth5, kPth6, kPth7,

		kPti0, kPti1, kPti2, kPti3, kPti4, kPti5, kPti6, kPti7,

		kDisable
	};

	struct Config {
		enum struct MuxControl {
			kAnalog, kGpio, kAlt2, kAlt3, kAlt4, kAlt5, kAlt6, kAlt7
		};

		enum struct Interrupt {
			kDisable,

			kDmaRising, kDmaFalling, kDmaBoth,

			kRising, kFalling, kBoth,

			kLow, kHigh
		};

		enum ConfigBit {
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
	explicit Pin (nullptr_t);
	Pin() :
			Pin(nullptr) {
	}
	Pin(const Pin&) = delete;
	Pin(Pin &&rhs);
	~Pin();

	Pin& operator=(const Pin&) = delete;
	Pin& operator=(Pin &&rhs);
	operator bool() const {
		return (m_name != Name::kDisable);
	}

	Name GetName() const {
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
