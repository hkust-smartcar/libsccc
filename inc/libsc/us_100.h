/*
 * us_100.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(uart)

namespace libsc
{

/**
 * Ultra-sonic sensor US-100, communicating through the UART
 */
class Us100
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit Us100(const Config &config);
	~Us100();

	/**
	 * Start shooting waves
	 */
	void Start();
	/**
	 * Stop shooting waves after finishing the current one
	 */
	void Stop();

	/**
	 * Return whether a reading is ready. This is only useful to check whether
	 * the first reading arrived or not as this method will always return true
	 * afterwards until the next Stop()/Start()
	 *
	 * @return
	 */
	bool IsAvailable() const
	{
		return m_is_available;
	}

	/**
	 * Return the latest distance reading, in mm
	 *
	 * @return
	 */
	uint16_t GetDistance() const
	{
		return m_distance;
	}

private:
	void OnRxFull(LIBBASE_MODULE(Uart) *uart);

	void Shoot();

	LIBBASE_MODULE(Uart) m_uart;

	volatile uint16_t m_distance;
	Byte m_buffer[2];
	int m_buffer_it;

	bool m_is_shoot;
	volatile bool m_is_shooting;
	volatile bool m_is_available;
};

}
