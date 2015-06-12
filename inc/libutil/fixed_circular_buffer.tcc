/*
 * fixed_circular_buffer.tcc
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

template<typename T>
FixedCircularBuffer<T>::FixedCircularBuffer(const size_t capacity)
		: m_capacity(capacity),
		  m_data(new T[capacity]),
		  m_start(0),
		  m_end(0)
{}

template<typename T>
bool FixedCircularBuffer<T>::PushData(T &&data)
{
	if (GetSize() == m_capacity)
	{
		return false;
	}
	else
	{
		m_data[m_end++ % m_capacity] = std::move(data);
		return true;
	}
}

template<typename T>
T* FixedCircularBuffer<T>::GetActiveData()
{
	if (GetSize() == 0)
	{
		return nullptr;
	}
	else
	{
		return &m_data[m_start % m_capacity];
	}
}

template<typename T>
T* FixedCircularBuffer<T>::NextData()
{
	if (GetSize() == 0)
	{
		return nullptr;
	}
	else
	{
		++m_start;
		return GetActiveData();
	}
}

}
