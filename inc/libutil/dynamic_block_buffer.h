/*
 * dynamic_block_buffer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>

#include <memory>
#include <string>
#include <vector>

#include "libbase/k60/misc_utils.h"

namespace libutil
{

class DynamicBlockBuffer
{
public:
	struct Block
	{
		Block(Byte* const data, const size_t size, const bool is_mem_owned)
				: type(kByteAry), size(size), it(0), is_mem_owned(is_mem_owned)
		{
			this->data.byte_ = data;
		}

		Block(Byte* const data, const size_t size)
				: Block(data, size, true)
		{}

		explicit Block(std::string* const data)
				: type(kString), size(data->size()), it(0)
		{
			this->data.string_ = data;
		}

		explicit Block(std::vector<Byte>* const data)
				: type(kVector), size(data->size()), it(0)
		{
			this->data.vector_ = data;
		}

		Block()
				: type(kByteAry), size(0), it(0), is_mem_owned(false)
		{
			this->data.byte_ = nullptr;
		}

		Block(const Block&) = delete;
		Block(Block &&rhs)
				: data(rhs.data), type(rhs.type), size(rhs.size), it(rhs.it),
				  is_mem_owned(rhs.is_mem_owned)
		{
			rhs.data.byte_ = nullptr;
			rhs.is_mem_owned = false;
		}

		~Block()
		{
			Recycle();
		}

		Block& operator=(const Block&) = delete;
		volatile Block& operator=(Block &&rhs) volatile;

		Block& operator=(Block &&rhs)
		{
			return *const_cast<Block*>(&const_cast<volatile Block*>(this)
					->operator=(std::move(rhs)));
		}

		void Recycle() volatile;

		union
		{
			Byte *byte_;
			std::string *string_;
			std::vector<Byte> *vector_;
		} data;
		enum : uint8_t
		{
			kByteAry,
			kString,
			kVector,
		} type;
		size_t size;
		uint8_t it;
		bool is_mem_owned = true;
	};

	explicit DynamicBlockBuffer(const size_t size);
	~DynamicBlockBuffer();

	void PushBuffer(Block &&slot);
	void Acquire();

	volatile Block* GetActiveBlock()
	{
		return m_active_block;
	}

	Uint GetSize() const
	{
		return (Uint)(m_queued - m_sent);
	}

private:
	const size_t m_size;
	const size_t m_upper_bound;
	const size_t m_lower_bound;

	std::unique_ptr<Block[]> m_blocks;
	volatile Block *m_active_block;
	volatile Uint m_queued;
	volatile Uint m_sent;
};

}
