/*
 * dac.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_DAC_H_
#define LIBBASE_K60_DAC_H_

#include <cstdint>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class Dac
{
public:
	struct Config
	{
		enum struct Source
		{
			// Currenlty not supported
			//kVref,
			kVdda,
		};

		enum struct BufferMode
		{
			// 0 -> MAX, 0 -> MAX, ...
			kNormal,
			// 0 -> MAX, MAX -> 0, ...
			kSwing,
			// 0 -> MAX
			kOneTimeScan,
		};

		// [0 - 1]
		uint8_t module;
		Source src = Source::kVdda;

		uint16_t data[16] = {};
		// [0 - 16]
		Uint data_size = 0;
		BufferMode buffer_mode = BufferMode::kNormal;
	};

	explicit Dac(const Config &config);
	explicit Dac(nullptr_t);
	Dac(const Dac&) = delete;
	Dac(Dac &&rhs);
	~Dac();

	Dac& operator=(const Dac&) = delete;
	Dac& operator=(Dac &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void SetData(const uint16_t *data, const Uint size,
			const Config::BufferMode buffer_mode);
	void SetData(const uint16_t *data, const Uint size)
	{
		SetData(data, size, Config::BufferMode::kNormal);
	}

	void SetData(const uint16_t data)
	{
		SetData(&data, 1);
	}

	void AdvanceData();
	void SetDataPosition(const Uint position);

private:
	void InitC0Reg(const Config &config);
	void Uninit();

	uint8_t m_module;
	bool m_is_init;
};

}
}

#endif /* LIBBASE_K60_DAC_H_ */
