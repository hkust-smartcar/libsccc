/*
 * fpu.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

namespace libbase
{
namespace k60
{

class Fpu
{
public:
	struct Config
	{
		enum struct RoundingMode
		{
			kNearest = 0,
			kPosInf,
			kNegInf,
			kZero,
		};

		// Enable the default NaN mode, see p.63 of DDI0403D
		bool is_default_nan = true;
		// Enable the flush-to-zero mode, which effectively discard subnormal
		// numbers, see p.62 of DDI0403D
		bool is_flush_to_zero = true;
		// Set the floating point rounding mode, has nothing to do with the
		// float->int conversion
		RoundingMode rounding_mode = RoundingMode::kNearest;
	};

	static Fpu& Get()
	{
		static Fpu inst;
		return inst;
	}

	void Init();

private:
	Fpu();

	__attribute__((__weak__))
	static Config GetFpuConfig();

	void InitFpccr(const Config &config);
	void InitFpdscr(const Config &config);
};

}
}
