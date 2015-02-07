/*
 * dma.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

/**
 * DMA controller. User should create a new DMA request through DmaManager class
 * instead of calling the constructor directly
 */
class Dma
{
public:
	typedef std::function<void(Dma *dma)> OnCompleteListener;
	typedef std::function<void(Dma *dma)> OnErrorListener;

	struct Config
	{
		enum struct TransferSize
		{
			k1Byte = 0,
			k2Byte = 1,
			k4Byte = 2,
			k16Byte = 4,
		};

		enum struct StallDuration
		{
			k0Cycle = 0,
			k4Cycle = 2,
			k8Cycle = 3,
		};

		struct NodeConfig
		{
			/// Memory address pointing to the data
			void *addr = nullptr;
			/**
			 * Sign-extended offset applied to the current address to form the
			 * next-state value as each read/write is completed
			 *
			 * @note Generally, Config::minor_bytes, NodeConfig::size and
			 * NodeConfig::offset are related
			 */
			int16_t offset;
			/**
			 * Data transfer size
			 *
			 * @note Generally, Config::minor_bytes, NodeConfig::size and
			 * NodeConfig::offset are related
			 */
			TransferSize size;
			/**
			 * Adjustment value added to the address at the completion of the
			 * major iteration
			 */
			int32_t major_offset;
		};

		struct MinorLoopConfig
		{
			bool is_enable_src_offset = false;
			bool is_enable_dst_offset = false;
			/// Minor loop offset, [-2^19, 2^19)
			int32_t offset;
		};

		NodeConfig src;
		NodeConfig dst;

		/**
		 * # of bytes trasferred per request. If minor loop mapping is disabled
		 * in DmaManager, the value should be [0, 2^32). If enabled, the value
		 * must be [0, 2^30) when both MinorLoopConfig::is_enable_src_offset and
		 * MinorLoopConfig::is_enable_dst_offset are false, otherwise [0, 2^10)
		 *
		 * @note Generally, Config::minor_bytes, NodeConfig::size and
		 * NodeConfig::offset are related
		 */
		uint32_t minor_bytes;
		/// Ignored if minor loop mapping is disabled
		MinorLoopConfig minor_loop;

		uint16_t major_count;
		/**
		 * Forces the DMA engine to stall after the completion of each
		 * read/write access to control the bus request bandwidth. By default it
		 * is a continuous operation
		 */
		StallDuration stall_duration = StallDuration::k0Cycle;

		/**
		 * If true, listener would also be called when the major iterations are
		 * halfly completed
		 */
		bool is_listen_half_complete;
		OnCompleteListener complete_isr;

		/// Enable error interrupt
		bool is_eror_irq = false;
		OnErrorListener error_isr;

		/// Disable DMA request after finishing major loop
		bool is_disable_request = true;
	};

	Dma(const Config &config, const Uint channel);
	explicit Dma(nullptr_t);
	Dma()
			: Dma(nullptr)
	{}
	Dma(const Dma&) = delete;
	Dma(Dma &&rhs);
	~Dma();

	Dma& operator=(const Dma&) = delete;
	Dma& operator=(Dma &&rhs);
	operator bool() const
	{
		return (m_channel != (Uint)-1);
	}

	void Start();
	/**
	 * Request the DMA engine to stop the transfer ASAP, listener won't be
	 * triggered
	 */
	void Stop();

	bool IsDone() const;
	void ResetDone();
	bool IsActive() const;

	Uint GetChannel() const
	{
		return m_channel;
	}

	void EnableInterrupt();
	void DisableInterrupt();

	static __ISR void IrqHandler();

private:
	void InitTcdAttrReg(const Config &config);
	void InitTcdNbytesReg(const Config &config);
	void InitTcdIterReg(const Config &config);
	void InitTcdCsrReg(const Config &config);

	void InitEeiReg(const Config &config);

	void Uninit();

	void ConsumeInterrupt();

	Uint m_channel;
	OnCompleteListener m_complete_isr;
	OnErrorListener m_error_isr;
};

}
}
