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

namespace libbase
{
namespace k60
{

struct PitConfig
{
	// [0 - 3]
	uint8_t channel;
	uint32_t count;
};

class Pit
{
public:
	typedef std::function<void(Pit *pit)> OnPitTriggerListener;

	Pit(const PitConfig &config, OnPitTriggerListener isr);
	explicit Pit(const PitConfig &config);
	explicit Pit(nullptr_t);
	~Pit();

	/**
	 * Enable/disable a PIT. Notice that PIT is always enabled after
	 * construction so you don't need to call this method explicitly
	 *
	 * @param flag
	 */
	void SetEnable(const bool flag);
	void SetIsr(OnPitTriggerListener isr);

	void SetCount(uint32_t count);
	uint32_t GetCountLeft() const;

	uint8_t GetChannel() const
	{
		return m_channel;
	}

private:
	static __ISR void IrqHandler();

	const uint8_t m_channel;
	OnPitTriggerListener m_isr;
};

}
}

#endif /* LIBBASE_K60_PIT_H_ */
