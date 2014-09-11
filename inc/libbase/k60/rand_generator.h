/*
 * rand_generator.h
 * Random Number Generator Accelerator
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace libbase
{
namespace k60
{

class RandGenerator
{
public:
	struct Config
	{
		// Set the external entropy, should also toggle is_ext_entropy to
		// respect this value
		uint32_t entropy;
		bool is_ext_entropy;
	};

	explicit RandGenerator(const Config &config);
	explicit RandGenerator(nullptr_t);
	RandGenerator(const RandGenerator&) = delete;
	RandGenerator(RandGenerator &&rhs);
	~RandGenerator();

	RandGenerator& operator=(const RandGenerator&) = delete;
	RandGenerator& operator=(RandGenerator &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void SetEntropy(const uint32_t entropy);

	bool IsReady() const;
	uint32_t GetRand() const;

private:
	void Uninit();

	bool m_is_init;
};

}
}
