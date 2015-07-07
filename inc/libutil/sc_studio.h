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
#include <memory>
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
	class GraphPack
	{
	public:
		explicit GraphPack(const size_t count);

		void Pack(const uint8_t id, const int32_t value);
		void PackF(const uint8_t id, const float value);

		size_t GetCount() const
		{
			return m_count;
		}

		const Byte* GetData() const
		{
			return m_data.get();
		}

	private:
		size_t m_count;
		std::unique_ptr<Byte[]> m_data;
		Byte *m_it;
	};

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
#if MK60DZ10 || MK60D10 || MK60F15
		return std::bind(&ScStudio::OnRx, this, std::placeholders::_1,
				std::placeholders::_2);
#elif MKL26Z4
		return std::bind(&ScStudio::OnRx1, this, std::placeholders::_1);
#endif
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
	void SendGraph(const GraphPack &pack);

private:
	bool OnRx(const Byte *data, const size_t size);
	bool OnRx1(const Byte data)
	{
		return OnRx(&data, 1);
	}

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
