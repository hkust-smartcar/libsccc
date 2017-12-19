/*
 * gpio.h
 *
 * Author: LeeChunHei
 * Copyright (c) 2017-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <bitset>
#include <functional>

#include "libbase/kea128/pin.h"

namespace libbase {
namespace kea128 {

class Gpo;

class Gpi {
public:
	typedef std::function<void(Gpi *gpi)> OnGpiEventListener;

	struct Config {
		Pin::Name pin;
		std::bitset<Pin::Config::ConfigBit::kSize> config;
		Pin::Config::Interrupt interrupt = Pin::Config::Interrupt::kDisable;
		OnGpiEventListener isr;
	};

	explicit Gpi(const Config &config);
	explicit Gpi (nullptr_t);
	Gpi() :
			Gpi(nullptr) {
	}
	Gpi(Gpi &&rhs);
	explicit Gpi(Pin &&rhs);
	~Gpi();

	Gpi& operator=(const Gpi&) = delete;
	Gpi& operator=(Gpi &&rhs);
	operator bool() const {
		return m_pin;
	}

	bool Get() const;
	Gpo ToGpo();

	Pin* GetPin() {
		return &m_pin;
	}

	const Pin* GetPin() const {
		return &m_pin;
	}

private:
	void Uninit();

	void SetIsr(const OnGpiEventListener &isr);

	Pin m_pin;
	Pin::Name m_pin_name;
	OnGpiEventListener m_isr;
};

class Gpo {
public:
	struct Config {
		Pin::Name pin;
		std::bitset<Pin::Config::ConfigBit::kSize> config;
		bool is_high = false;
	};

	explicit Gpo(const Config &config);
	explicit Gpo (nullptr_t);
	Gpo() :
			Gpo(nullptr) {
	}
	Gpo(const Gpo&) = delete;
	Gpo(Gpo &&rhs);
	explicit Gpo(Pin &&rhs);
	~Gpo();

	Gpo& operator=(const Gpo&) = delete;
	Gpo& operator=(Gpo &&rhs);
	operator bool() const {
		return m_pin;
	}

	void Set(bool is_high);
	void Turn();
	bool Get() const;

	void Set() {
		Set(true);
	}

	void Reset() {
		Set(false);
	}

	void Clear() {
		Reset();
	}

	Gpi ToGpi();

	/**
	 * Config this Gpo up to be ready to serve toggle as DMA destination, and
	 * set @a config accordingly. The following options are also set besides
	 * dst:<br>
	 * Dma::Config::minor_bytes = 4<br>
	 * Dma::Config::major_count = 1
	 *
	 * @param config
	 */
	void ConfigToggleAsDmaDst(Dma::Config *config);

	Pin* GetPin() {
		return &m_pin;
	}

private:
	void Uninit();

	Pin m_pin;
};

class Gpio {
public:
	/**
	 * Construct a GPIO pin where initially functions as an input pin. Interrupt
	 * is not supported in this setup.
	 *
	 * @param config
	 */
	explicit Gpio(const Gpi::Config &config);
	explicit Gpio(const Gpo::Config &config);
	explicit Gpio (nullptr_t);
	Gpio(const Gpio&) = delete;
	Gpio(Gpio &&rhs);
	~Gpio();

	Gpio& operator=(const Gpio&) = delete;
	Gpio& operator=(Gpio &&rhs);
	operator bool() const {
		return (m_gpi || m_gpo);
	}

	void Set(bool is_high) {
		EnsureGpo();
		m_gpo.Set(is_high);
	}

	void Turn() {
		EnsureGpo();
		m_gpo.Turn();
	}

	void Set() {
		Set(true);
	}

	void Reset() {
		Set(false);
	}

	void Clear() {
		Reset();
	}

	bool GetOutput() {
		EnsureGpo();
		return m_gpo.Get();
	}

	bool Get() {
		EnsureGpi();
		return m_gpi.Get();
	}

	void EnsureGpi();
	void EnsureGpo();

	bool IsOutput() const {
		return m_is_gpo;
	}

private:
	void Uninit();

	Gpi m_gpi;
	Gpo m_gpo;
	bool m_is_gpo;
};

}
}
