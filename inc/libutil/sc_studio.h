/*
 * sc_studio.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <cstring>

#include <array>
#include <functional>
#include <string>
#include <vector>

#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(uart)

#include "libsc/tsl1401cl.h"
#include LIBSC_H(uart_device)

namespace libutil
{

class ScStudio
{
public:
	enum struct MessageToken
	{
		kNull = -1,
		kNop = 0,
		kString,
		kCcdData,
		kCamera,
		kGraph,

		kSize
	};

	ScStudio();

	void SetUart(LIBSC_MODULE(UartDevice) *uart)
	{
		m_uart = uart;
	}

	LIBSC_MODULE(UartDevice)::OnReceiveListener GetUartListener()
	{
		return std::bind(&ScStudio::OnRx, this, std::placeholders::_1,
				std::placeholders::_2);
	}

	void SendString(const char *str, const size_t size);
	void SendString(const char *str)
	{
		SendString(str, strlen(str));
	}
	void SendString(std::string &&str);
	void SendCcdData(const uint8_t id,
			const std::array<uint16_t, libsc::Tsl1401cl::kSensorW> &data);
	void SendCamera(const Byte *data, const size_t size);
	void SendGraph(const uint8_t id, const int32_t value);
	void SendGraphF(const uint8_t id, const float value);

private:
	bool OnRx(const Byte *data, const size_t size);

	void OnNewMessage();

	void SendRaw(const MessageToken token, const uint32_t size, const Byte *data);
	void Reset();

	static constexpr Byte kBegin = 0xDC;
	static constexpr Byte kEnd = 0xCD;

	bool m_is_start;
	MessageToken m_token;
	int m_size;
	Byte m_buf[255];
	uint8_t m_buf_it;

	LIBSC_MODULE(UartDevice) *m_uart;
};

}
