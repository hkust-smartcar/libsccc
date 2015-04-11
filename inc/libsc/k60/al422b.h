/*
 * al422b.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>

#include <vector>

#include "libbase/misc_types.h"
#include "libbase/helper.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(gpio_array)

namespace libsc
{
namespace k60
{

/**
 * Read-only AL422B FIFO memory with a free-running WCLK provided, and 8 output
 * pins. WE, RE and OE pins are optional and if not set, should be pulled low
 */
class Al422b
{
public:
	struct Config
	{
		/// Write enable, optional
		LIBBASE_MODULE(Pin)::Name we = LIBBASE_MODULE(Pin)::Name::kDisable;
		/// Write reset
		LIBBASE_MODULE(Pin)::Name wrst;
		/// Data output pin 0
		LIBBASE_MODULE(Pin)::Name do0;
		/// Read clock
		LIBBASE_MODULE(Pin)::Name rck;
		/// Read enable, optional
		LIBBASE_MODULE(Pin)::Name re = LIBBASE_MODULE(Pin)::Name::kDisable;
		/// Read reset
		LIBBASE_MODULE(Pin)::Name rrst;
		/// Output enable
		LIBBASE_MODULE(Pin)::Name oe = LIBBASE_MODULE(Pin)::Name::kDisable;
		/**
		 * The initial size of the internal buffer, mainly used to pre-allocate
		 * memory here instead of during Al422b::Start(). Optional
		 */
		size_t initial_size = 0;
	};

	explicit Al422b(const Config &config);
	explicit Al422b(nullptr_t);
	~Al422b();

	void Start(const uint32_t size);
	/**
	 * Read 1 byte. If the data is filled, true will be returned, false
	 * otherwise
	 *
	 * @return
	 */
	bool ReadStep();

	const std::vector<Byte>& GetData() const
	{
		return m_data;
	}

	size_t GetDataSize() const
	{
		return m_data.size();
	}

	void ResetWrite();
	void ResetRead();

private:
	LIBBASE_MODULE(Gpo) m_we;
	LIBBASE_MODULE(Gpo) m_wrst;
	LIBBASE_MODULE(GpiArray) m_do;
	LIBBASE_MODULE(Gpo) m_rck;
	LIBBASE_MODULE(Gpo) m_re;
	LIBBASE_MODULE(Gpo) m_rrst;
	LIBBASE_MODULE(Gpo) m_oe;

	std::vector<Byte> m_data;
	std::vector<Byte>::iterator m_it;
};

}
}
