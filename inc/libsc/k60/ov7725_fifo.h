/*
 * ov7725_fifo.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <vector>

#include "libbase/k60/gpio.h"
#include "libbase/misc_types.h"

#include "libsc/k60/al422b.h"
#include "libsc/k60/ov7725.h"
#include "libsc/k60/ov7725_configurator.h"

namespace libsc
{
namespace k60
{

/**
 * OV7725 + FIFO camera
 */
class Ov7725Fifo
{
public:
	typedef Ov7725::Config Config;

	explicit Ov7725Fifo(const Config &config);
	~Ov7725Fifo();

	/**
	 * Start shooting a frame. No effect if the previous frame has not yet
	 * finished reading
	 *
	 * @see ReadStep()
	 */
	void Start();
	/**
	 * @see Al422b::ReadStep()
	 */
	bool ReadStep();

	/**
	 * Return the raw data, in RGB565 (so 2 bytes form 1 pixel)
	 *
	 * @return
	 * @note On LE platform, you can't simply cast it to uint16_t*
	 */
	const std::vector<Byte>& GetData() const
	{
		return m_fifo.GetData();
	}

	/**
	 * Return the transformed data, as a RGB565 array
	 *
	 * @return
	 */
	std::vector<uint16_t> GetRgb565Data() const;

	Uint GetW() const
	{
		return m_w;
	}

	Uint GetH() const
	{
		return m_h;
	}

private:
	enum State
	{
		kIdle,
		kReqStart,
		kStart,
	};

	void OnVsync(libbase::k60::Gpi *gpi);

	Ov7725Configurator m_config;
	Al422b m_fifo;
	libbase::k60::Gpo m_wen;
	libbase::k60::Gpi m_vsync;

	Uint m_w;
	Uint m_h;

	volatile State m_write_state;
};

}
}
