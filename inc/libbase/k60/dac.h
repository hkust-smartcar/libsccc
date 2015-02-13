/*
 * dac.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class Dac
{
public:
	enum struct Name
	{
		kDac0 = 0,
		kDac1,

		kDisable
	};

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

		Dac::Name module;
		Source src = Source::kVdda;

		uint16_t data[16] = {};
		// [1 - 16]
		Uint data_size;
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
		return (m_name != Dac::Name::kDisable);
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

	Name m_name;
};

}
}
