/*
 * pit.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_PIT_H_
#define LIBBASE_K60_PIT_H_

#include <vectors.h>

#include <cstdint>
#include <functional>

#include "libbase/k60/delay.h"

namespace libbase
{
namespace k60
{

class Pit
{
public:
	typedef std::function<void(Pit *pit)> OnPitTriggerListener;

	struct Config
	{
		// [0 - 3]
		uint8_t channel;
		// Arbitrary default count
		uint32_t count = 10000;
		OnPitTriggerListener isr;
		bool is_enable = true;
	};

	explicit Pit(const Config &config);
	explicit Pit(nullptr_t);
	Pit(const Pit&) = delete;
	Pit(Pit &&rhs);
	~Pit();

	Pit& operator=(const Pit&) = delete;
	Pit& operator=(Pit &&rhs);

	void SetEnable(const bool flag);

	void SetCount(uint32_t count);
	uint32_t GetCountLeft() const;

	bool IsInterruptRequested() const;

	uint8_t GetChannel() const
	{
		return m_channel;
	}

private:
	void SetIsr(const OnPitTriggerListener &isr);
	void Uninit();

	static __ISR void IrqHandler();

	uint8_t m_channel;
	OnPitTriggerListener m_isr;
	bool m_is_init;
};

class PitDelay : public Delay
{
public:
	explicit PitDelay(const uint8_t channel);

	void DelayUs(const uint16_t us) override;
	void DelayMs(const uint16_t ms) override;

private:
	Pit m_pit;
};

}
}

#endif /* LIBBASE_K60_PIT_H_ */
