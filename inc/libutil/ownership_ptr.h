/*
 * ownership_ptr.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>

#include <memory>

namespace libutil
{

/**
 * A managed pointer that might or might not be owned
 */
template<typename T>
class OwnershipPtr
{
public:
	OwnershipPtr()
			: m_ptr(nullptr),
			  m_is_owned(false)
	{}

	OwnershipPtr(T *const ptr, const bool is_owned)
			: m_ptr(ptr),
			  m_is_owned(is_owned)
	{}

	OwnershipPtr(T *const ptr)
			: m_ptr(ptr),
			  m_is_owned(false)
	{}

	explicit OwnershipPtr(std::unique_ptr<T> &&ptr)
			: m_ptr(ptr.release()),
			  m_is_owned(true)
	{}

	OwnershipPtr(const OwnershipPtr &rhs) = delete;

	OwnershipPtr(OwnershipPtr &&rhs)
			: m_ptr(rhs.m_ptr)
	{
		const bool is_owned = rhs.m_is_owned;
		rhs.m_is_owned = false;
		m_is_owned = is_owned;
	}

	~OwnershipPtr()
	{
		if (m_is_owned)
		{
			delete m_ptr;
		}
	}

	T* operator->() const
	{
		return get();
	}

	T& operator*() const
	{
		return *get();
	}

	T* get() const
	{
		return m_ptr;
	}

	T* release() const
	{
		T *ptr = get();
		m_ptr = nullptr;
		m_is_owned = false;
		return ptr;
	}

private:
	T *m_ptr;
	bool m_is_owned;
};

template<typename T>
class OwnershipPtr<T[]>
{
public:
	OwnershipPtr()
			: m_ptr(nullptr),
			  m_is_owned(false)
	{}

	OwnershipPtr(T *const ptr, const bool is_owned)
			: m_ptr(ptr),
			  m_is_owned(is_owned)
	{}

	OwnershipPtr(T *const ptr)
			: m_ptr(ptr),
			  m_is_owned(false)
	{}

	explicit OwnershipPtr(std::unique_ptr<T> &&ptr)
			: m_ptr(ptr.release()),
			  m_is_owned(true)
	{}

	OwnershipPtr(const OwnershipPtr &rhs) = delete;

	OwnershipPtr(OwnershipPtr &&rhs)
			: m_ptr(rhs.m_ptr)
	{
		const bool is_owned = rhs.m_is_owned;
		rhs.m_is_owned = false;
		m_is_owned = is_owned;
	}

	~OwnershipPtr()
	{
		if (m_is_owned)
		{
			delete[] m_ptr;
		}
	}

	T* operator->() const
	{
		return get();
	}

	T& operator*() const
	{
		return *get();
	}

	T& operator[](const size_t position)
	{
		return get()[position];
	}

	const T& operator[](const size_t position) const
	{
		return get()[position];
	}

	T* get() const
	{
		return m_ptr;
	}

	T* release() const
	{
		T *ptr = get();
		m_ptr = nullptr;
		m_is_owned = false;
		return ptr;
	}

private:
	T *m_ptr;
	bool m_is_owned;
};

}
