/*
 * spi_device.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/spi_master.h"

#include "libsc/k60/spi_device.h"
#include "libutil/dynamic_block_buffer.h"

using namespace libbase::k60;
using namespace libutil;
using namespace std;

#define BLOCK_SIZE 64

namespace libsc
{
namespace k60
{

namespace
{

SpiMaster::Config GetSpiConfig(const SpiMaster::Config &config,
		const SpiMaster::OnTxFillListener &tx_isr,
		const SpiMaster::OnRxDrainListener &rx_isr)
{
	SpiMaster::Config product = config;
	product.tx_isr = tx_isr;
	product.rx_isr = rx_isr;
	return product;
}

}

SpiDevice::SpiDevice(const SpiMaster::Config &config)
		: m_tx_buf(28),
		  m_spi(GetSpiConfig(config,
				  std::bind(&SpiDevice::OnTxFill, this, placeholders::_1),
				  std::bind(&SpiDevice::OnRxDrain, this, placeholders::_1)))
{}

SpiDevice::SpiDevice(SpiDevice&&)
		: SpiDevice(nullptr)
{
	// TODO
	//*this = std::move(rhs);
}

SpiDevice::SpiDevice(nullptr_t)
		: m_tx_buf(0),
		  m_spi(nullptr)
{}

SpiDevice::~SpiDevice()
{}

void SpiDevice::SendBuffer(const Byte *buf, const size_t len)
{
	if (len == 0)
	{
		return;
	}
	Byte *data = new Byte[len];
	memcpy(data, buf, len);

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(data, len));
	m_spi.KickStart();
}

void SpiDevice::SendBuffer(unique_ptr<Byte[]> &&buf, const size_t len)
{
	if (len == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(buf.release(), len));
	m_spi.KickStart();
}

void SpiDevice::SendBuffer(vector<Byte> &&buf)
{
	if (buf.size() == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(new vector<Byte>(std::move(buf))));
	m_spi.KickStart();
}

void SpiDevice::OnTxFill(SpiMaster *spi)
{
	volatile DynamicBlockBuffer::Block *block = m_tx_buf.GetActiveBlock();
	if (m_tx_buf.GetSize() == 0 && (!block || block->it == block->size))
	{
		return;
	}

	while (!block || block->it == block->size)
	{
		m_tx_buf.Acquire();
		block = m_tx_buf.GetActiveBlock();
	}

	const size_t size = block->size - block->it;
	switch (block->type)
	{
	case DynamicBlockBuffer::Block::kByteAry:
		block->it += spi->PushData(0, block->data.byte_ + block->it, size);
		break;

	case DynamicBlockBuffer::Block::kString:
		block->it += spi->PushData(0, (const Byte*)block->data.string_->data()
				+ block->it, size);
		break;

	case DynamicBlockBuffer::Block::kVector:
		block->it += spi->PushData(0, block->data.vector_->data() + block->it,
				size);
		break;
	}
}

void SpiDevice::OnRxDrain(SpiMaster*)
{
	// TODO
}

}
}
