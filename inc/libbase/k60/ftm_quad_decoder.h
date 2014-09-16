/*
 * ftm_quad_decoder.h
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
#include "libbase/k60/quad_decoder_interface.h"

namespace libbase
{
namespace k60
{

class FtmQuadDecoder : public QuadDecoderInterface
{
public:
	typedef std::function<void(FtmQuadDecoder *uart)> OnOverflowListener;

	struct Config : public QuadDecoderInterface::Config
	{
		enum struct EncodingMode
		{
			/**
			 * In this mode, the relationship between phase A and B signals
			 * indicates the counting direction, and phase A and B signals
			 * define the counting rate (FTM counter is updated when there is an
			 * edge either at the phase A or phase B signals)
			 */
			kPhaseAB,

			/**
			 * In this mode, the phase B input value indicates the counting
			 * direction (FTM counter increment or decrement), and the phase A
			 * input defines the counting rate (FTM counter is updated when
			 * there is a rising edge at phase A input signal)
			 */
			kCountDirection,
		};

		EncodingMode encoding_mode;
		/**
		 * Any pulse that is shorter than the value * 4 system clocks is
		 * regarded as a glitch and is not passed on to the edge detector,
		 * [0, 15]
		 */
		uint8_t a_filter_length = 0;
		uint8_t b_filter_length = 0;
		/**
		 * The value where overflow occurs. This value will be ignored if
		 * #overflow_isr is not being set, [2, UINT16_MAX]
		 */
		uint16_t overflow_count = UINT16_MAX;
		/**
		 * The listener will be invoked when counted to #overflow_count * n or
		 * -(#overflow_count + 1) * n
		 */
		OnOverflowListener overflow_isr;
	};

	explicit FtmQuadDecoder(const Config &config);
	explicit FtmQuadDecoder(nullptr_t);
	FtmQuadDecoder(const FtmQuadDecoder&) = delete;
	FtmQuadDecoder(FtmQuadDecoder &&rhs);
	~FtmQuadDecoder();

	FtmQuadDecoder& operator=(const FtmQuadDecoder&) = delete;
	FtmQuadDecoder& operator=(FtmQuadDecoder &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	int32_t GetCount() override;
	void ResetCount() override;

private:
	bool InitModule(const Pin::Name a_pin, const Pin::Name b_pin);
	void InitPin(const Pin::Name a_pin, const Pin::Name b_pin);
	void InitCounter(const Config &cofig);
	void InitScReg(const Config &config);
	void InitQdctrlReg(const Config &config);
	void InitConfReg(const Config &config);
	void InitInputFilter(const Config &config);
	void InitInterrupt(const Config &config);

	void Uninit();

	void SetReadOnlyReg(const bool flag);
	void SetInterrupt(const bool flag);

	static __ISR void TofIrqHandler();

	uint8_t m_module;

	Pin m_a;
	Pin m_b;

	int m_overflow;
	uint16_t m_mod;
	OnOverflowListener m_overflow_isr;

	bool m_is_init;
};

}
}
