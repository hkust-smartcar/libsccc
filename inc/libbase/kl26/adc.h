/*
 * adc.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout_macros.h"

namespace libbase
{
namespace kl26
{

class Adc
{
public:
	typedef std::function<void(Adc *adc, const uint16_t result)>
			OnConversionCompleteListener;

	enum struct Name
	{
		kAdc0DAd0 = 0,
		kAdc0DAd1,
		kAdc0DAd2,
		kAdc0DAd3,
		kAdc0Ad4A,
		kAdc0Ad5A,
		kAdc0Ad6A,
		kAdc0Ad7A,
		kAdc0Ad4B,
		kAdc0Ad5B,
		kAdc0Ad6B,
		kAdc0Ad7B,
		kAdc0Ad8,
		kAdc0Ad9,
		kAdc0Ad10,
		kAdc0Ad11,
		kAdc0Ad12,
		kAdc0Ad13,
		kAdc0Ad14,
		kAdc0Ad15,
		kAdc0Ad16,
		kAdc0Ad17,
		kAdc0Ad18,
		kAdc0Ad19,
		kAdc0Ad20,
		kAdc0Ad21,
		kAdc0Ad22,
		kAdc0Ad23,
		kAdc0Ad24,
		kAdc0Ad25,
		kAdc0Ad26,
		kAdc0Ad27,
		kAdc0Ad28,
		kAdc0Ad29,
		kAdc0Ad30,
		kAdc0Ad31,

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

		///@{
		/**
		 * Specify the pin to be used. Either Config::pin or Config::adc has to
		 * be set. If Config::pin is set, Config::adc will be ignored. This
		 * provides two ways to specify the ADC pin as some pins can be used
		 * with more than 1 ADC channel
		 */
		Pin::Name pin = Pin::Name::kDisable;
		Adc::Name adc = Adc::Name::kDisable;
		/// @}
		/// Enable diferential mode, only work with DADn channels
		bool is_diff_mode = false;
		Resolution resolution = Resolution::k8Bit;
		SpeedMode speed = SpeedMode::kTypical;
		bool is_continuous_mode = false;
		AveragePass avg_pass = AveragePass::kDisable;
		/**
		 * @note Having listener and continuous mode both set will most likely
		 * result in the listener being called in a dead lock manner
		 */
		OnConversionCompleteListener conversion_isr;
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
	 * Return the raw result of a single conversion. If a conversion has not
	 * been started yet, it will be started automatically (through StartConvert())
	 *
	 * @return
	 */
	uint16_t GetResult();
	/**
	 * Return the divided result of a single conversion. If a conversion has not
	 * been started yet, it will be started automatically (through StartConvert())
	 *
	 * @return The result, [0.0, 3.3]
	 */
	float GetResultF();
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
