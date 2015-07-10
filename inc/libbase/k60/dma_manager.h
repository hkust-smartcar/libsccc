/*
 * dma_manager.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <bitset>

#include "libbase/k60/dma.h"
#include "libbase/k60/pinout_macros.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace k60
{

class DmaManager
{
public:
	struct Config
	{
		struct ChannelPriorityConfig
		{
			// Can this channel be suspended by a higher priority channel
			bool is_channel_preemption = false;
			// Can this channel suspend a lower priority channel
			bool is_preempt_low_channel = false;
			uint8_t priority;
		};

		enum ConfigBit
		{
			kMinorLoopMapping = 0,
			/**
			 * If set, a minor loop channel link made to itself does not go
			 * through channel arbitration before being activated again upon
			 * minor loop completion. This effectively applies the minor loop
			 * offsets and restarts the next minor loop
			 */
			kContinuousLinkMode,
			kHaltOnError,
			/// Use Round Robin instead of fixed-priority channel arbitration
			kRoundRobinChannelArbitration,

			kSize
		};

		Config();

		std::bitset<ConfigBit::kSize> config;
		/**
		 * When fixed-priority channel arbitration is enabled, this array is
		 * used to define the unique priorities associated with each channel.
		 * The channel priorities must be unique and range from [0, DMA_COUNT)
		 * where 0 indicates the lowest priority. By default, all channels'
		 * priorities are assigned as their channel #
		 *
		 * When Round Robin is enabled instead, this array is ignored
		 */
		ChannelPriorityConfig ch_configs[PINOUT_DMA_CH_COUNT];
	};

	/**
	 * Initialize the DMA manager, could ONLY be called before starting the 1st
	 * DMA process, otherwise the call would fail. If no Init() is invoked, the
	 * default config would be used
	 *
	 * @param config
	 */
	static void Init(const Config &config)
	{
		GetInstance()->Init_(config);
	}

	/**
	 * Start a DMA transfer in a specific channel
	 *
	 * @param dma
	 * @param channel
	 */
	static Dma* New(const Dma::Config &dma_config, const Uint channel)
	{
		return GetInstance()->New_(dma_config, channel);
	}

	static void Delete(Dma *dma)
	{
		GetInstance()->Delete_(dma);
	}

	static Dma* GetChannel(const Uint channel)
	{
		return GetInstance()->GetChannel_(channel);
	}

	static bool IsMinorLoopMapping()
	{
		return GetInstance()->m_is_minor_loop_mapping;
	}

private:
	struct ChannelData
	{
		Dma dma;
	};

	DmaManager();

	static DmaManager* GetInstance();

	void Init_(const Config &config);
	void InitCrReg(const Config &config);
	void InitDchPriReg(const Config &config);

	void Init_()
	{
		Init_(Config());
	}

	void EnsureInit()
	{
		if (!m_is_init)
		{
			Init_();
		}
	}

	Dma* New_(const Dma::Config &dma_config, const Uint channel);
	void Delete_(Dma *dma);
	Dma* GetChannel_(const Uint channel);

	bool m_is_init;
	bool m_is_minor_loop_mapping;
	ChannelData m_channels[PINOUT_DMA_CH_COUNT];

	static DmaManager *m_instance;
};

}
}
