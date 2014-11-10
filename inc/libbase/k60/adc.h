/*
 * adc.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout.h"

namespace libbase
{
namespace k60
{

class Adc
{
public:
	typedef std::function<void(Adc *adc, const uint16_t result)>
			OnConversionCompleteListener;

	enum struct Name
	{
		kAdc0DAd0 = 0, kAdc0DAd1, kAdc0DAd2, kAdc0DAd3,
		kAdc0Ad4A, kAdc0Ad5A, kAdc0Ad6A, kAdc0Ad7A,
		kAdc0Ad4B, kAdc0Ad5B, kAdc0Ad6B, kAdc0Ad7B,
		kAdc0Ad8, kAdc0Ad9, kAdc0Ad10, kAdc0Ad11, kAdc0Ad12,
		kAdc0Ad13, kAdc0Ad14, kAdc0Ad15, kAdc0Ad16, kAdc0Ad17,
		kAdc0Ad18, kAdc0Ad19, kAdc0Ad20, kAdc0Ad21, kAdc0Ad22,
		kAdc0Ad23, kAdc0Ad24, kAdc0Ad25, kAdc0Ad26, kAdc0Ad27,
		kAdc0Ad28, kAdc0Ad29, kAdc0Ad30, kAdc0Ad31,

		kAdc1DAd0 = 36, kAdc1DAd1, kAdc1DAd2, kAdc1DAd3,
		kAdc1Ad4A, kAdc1Ad5A, kAdc1Ad6A, kAdc1Ad7A,
		kAdc1Ad4B, kAdc1Ad5B, kAdc1Ad6B, kAdc1Ad7B,
		kAdc1Ad8, kAdc1Ad9, kAdc1Ad10, kAdc1Ad11, kAdc1Ad12,
		kAdc1Ad13, kAdc1Ad14, kAdc1Ad15, kAdc1Ad16, kAdc1Ad17,
		kAdc1Ad18, kAdc1Ad19, kAdc1Ad20, kAdc1Ad21, kAdc1Ad22,
		kAdc1Ad23, kAdc1Ad24, kAdc1Ad25, kAdc1Ad26, kAdc1Ad27,
		kAdc1Ad28, kAdc1Ad29, kAdc1Ad30, kAdc1Ad31,

#if PINOUT_ADC_COUNT > 2
		kAdc2DAd0 = 72, kAdc2DAd1, kAdc2DAd2, kAdc2DAd3,
		kAdc2Ad4A, kAdc2Ad5A, kAdc2Ad6A, kAdc2Ad7A,
		kAdc2Ad4B, kAdc2Ad5B, kAdc2Ad6B, kAdc2Ad7B,
		kAdc2Ad8, kAdc2Ad9, kAdc2Ad10, kAdc2Ad11, kAdc2Ad12,
		kAdc2Ad13, kAdc2Ad14, kAdc2Ad15, kAdc2Ad16, kAdc2Ad17,
		kAdc2Ad18, kAdc2Ad19, kAdc2Ad20, kAdc2Ad21, kAdc2Ad22,
		kAdc2Ad23, kAdc2Ad24, kAdc2Ad25, kAdc2Ad26, kAdc2Ad27,
		kAdc2Ad28, kAdc2Ad29, kAdc2Ad30, kAdc2Ad31,
#endif

#if PINOUT_ADC_COUNT > 3
		kAdc3DAd0 = 108, kAdc3DAd1, kAdc3DAd2, kAdc3DAd3,
		kAdc3Ad4A, kAdc3Ad5A, kAdc3Ad6A, kAdc3Ad7A,
		kAdc3Ad4B, kAdc3Ad5B, kAdc3Ad6B, kAdc3Ad7B,
		kAdc3Ad8, kAdc3Ad9, kAdc3Ad10, kAdc3Ad11, kAdc3Ad12,
		kAdc3Ad13, kAdc3Ad14, kAdc3Ad15, kAdc3Ad16, kAdc3Ad17,
		kAdc3Ad18, kAdc3Ad19, kAdc3Ad20, kAdc3Ad21, kAdc3Ad22,
		kAdc3Ad23, kAdc3Ad24, kAdc3Ad25, kAdc3Ad26, kAdc3Ad27,
		kAdc3Ad28, kAdc3Ad29, kAdc3Ad30, kAdc3Ad31,
#endif

		kDisable
	};

	struct Config
	{
		enum struct Resolution
		{
			k8Bit,
			k10Bit,
			k12Bit,
			k16Bit,
		};

		enum struct SpeedMode
		{
			kExFast,
			kFast,
			kTypical,
			kSlow,
			kExSlow,
		};

		enum struct AveragePass
		{
			kDisable = 0,
			k4,
			k8,
			k16,
			k32,
		};

		// Either pin or adc has to be set. If pin is set, adc will be ignored.
		// This provides two ways to specify the ADC pin. If the pin can be used
		// with more than 1 ADC module (eg, PTB0, PTB1), the first one will be
		// chosen by default. Otherwise you should specify the ADC pin through
		// adc
		Pin::Name pin = Pin::Name::kDisable;
		Adc::Name adc = Adc::Name::kDisable;
		// Enable diferential mode, only work with DADn channels
		bool is_diff_mode = false;
		Resolution resolution = Resolution::k8Bit;
		SpeedMode speed = SpeedMode::kTypical;
		bool is_continuous_mode = false;
		AveragePass avg_pass = AveragePass::kDisable;
		// Having listener and continuous mode both set will most likely result
		// in the listener being called in a dead lock manner
		OnConversionCompleteListener conversion_listener;
	};

	static constexpr int kModuleChannelCount = 36;

	explicit Adc(const Config &config);
	explicit Adc(nullptr_t);
	Adc(const Adc&) = delete;
	Adc(Adc &&rhs);
	~Adc();

	Adc& operator=(const Adc&) = delete;
	Adc& operator=(Adc &&rhs);
	operator bool() const
	{
		return (m_name != Adc::Name::kDisable);
	}

	void StartConvert();
	void StopConvert();

	/**
	 * Return the result of a single conversion. If a conversion has not been
	 * started yet, it will be started automatically (through StartConvert()).
	 *
	 * @return The result
	 */
	uint16_t GetResult();
	bool PeekResult(uint16_t *out_val);

	bool IsActive() const;

	/**
	 * Set the speed mode, effective from the next call to StartConvert()
	 *
	 * @param speed
	 */
	void SetSpeedMode(const Config::SpeedMode speed)
	{
		m_config.speed = speed;
	}

	/**
	 * Set the hardware average pass, effective from the next call to
	 * StartConvert()
	 *
	 * @param speed
	 */
	void SetAveragePass(const Config::AveragePass avg_pass)
	{
		m_config.avg_pass = avg_pass;
	}

private:
	bool InitModule(const Pin::Name pin);
	bool InitModule(const Adc::Name adc);
	bool InitPin(const Pin::Name pin);

	void InitSc1Reg();
	void InitCfg1Reg();
	void InitCfg2Reg();
	void InitSc3Reg();
	void InitSpeed();
	void InitInterrupt();

	void Uninit();

	bool IsConversionActive() const;
	bool IsConversionComplete() const;

	void EnableInterrupt();
	static void DisableInterrupt(const Uint module);

	static __ISR void IrqHandler();

	Name m_name;
	Config m_config;

	Pin m_pin;
};

}
}
