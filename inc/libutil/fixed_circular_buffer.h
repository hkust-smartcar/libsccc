/*
 * fixed_circular_buffer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <memory>

namespace libutil
{

/**
 * A fixed size buffer that would reject adding new data if old data are not
 * being consumed fast enough
 */
template<typename T>
class FixedCircularBuffer
{
public:
	explicit FixedCircularBuffer(const size_t capacity);

	uint32_t GetSize() const
	{
		return (uint32_t)(m_end - m_start);
	}

	bool PushData(T &&data);
	T* GetActiveData();
	T* NextData();

private:
	const size_t m_capacity;
	std::unique_ptr<T[]> m_data;
	uint32_t m_start;
	uint32_t m_end;
};

}

#include "fixed_circular_buffer.tcc"
