/*
 * dynamic_block_buffer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstddef>

#include <memory>
#include <string>
#include <vector>

#include "libbase/k60/misc_utils.h"

#include "libutil/dynamic_block_buffer.h"

using namespace std;

namespace libutil
{

volatile DynamicBlockBuffer::Block& DynamicBlockBuffer::Block::operator=(
		Block &&rhs) volatile
{
	if (this != &rhs)
	{
		Byte* const data_ = rhs.data.byte_;
		bool is_mem_owned_ = rhs.is_mem_owned;
		Recycle();
		rhs.data.byte_ = nullptr;
		rhs.is_mem_owned = false;

		data.byte_ = data_;
		type = rhs.type;
		size = rhs.size;
		it = rhs.it;
		is_mem_owned = is_mem_owned_;
	}
	return *this;
}

void DynamicBlockBuffer::Block::Recycle() volatile
{
	if (is_mem_owned && data.byte_)
	{
		switch (type)
		{
		case kByteAry:
			delete[] data.byte_;
			break;

		case kString:
			delete data.string_;
			break;

		case kVector:
			delete data.vector_;
			break;
		}
	}
}

DynamicBlockBuffer::DynamicBlockBuffer(const size_t size)
		: m_size(size),
		  m_upper_bound(size + 2),
		  m_lower_bound(size / 2),

		  m_blocks({new Block[m_upper_bound]}),
		  m_active_block(nullptr),
		  m_queued(0),
		  m_sent(0)
{}

DynamicBlockBuffer::~DynamicBlockBuffer()
{}

void DynamicBlockBuffer::PushBuffer(Block &&slot)
{
	if (GetSize() > m_size)
	{
		while (GetSize() > m_lower_bound)
		{
			++m_sent;
		}
	}
	Block *s;
	while ((s = &m_blocks[m_queued++ % m_upper_bound]) == m_active_block)
	{}
	*s = std::move(slot);
}

void DynamicBlockBuffer::Acquire()
{
	m_active_block = &m_blocks[m_sent++ % m_upper_bound];
}

}
