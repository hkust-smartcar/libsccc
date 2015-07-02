/*
 * sc_studio.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>
#include <cstring>

#include <array>
#include <functional>
#include <string>
#include <vector>

#include "libbase/log.h"
#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(uart)

#include "libsc/tsl1401cl.h"
#include LIBSC_H(uart_device)
#include "libutil/sc_studio.h"

using namespace LIBBASE_NS;
using namespace libsc;
using namespace LIBSC_NS;
using namespace std;

namespace libutil
{

ScStudio::ScStudio()
		: m_uart(nullptr)
{
	Reset();
}

void ScStudio::SendString(const char *str, const size_t size)
{
	SendRaw(MessageToken::kString, size, (Byte*)str);
}

void ScStudio::SendString(string &&str)
{
	SendRaw(MessageToken::kString, str.size(), (Byte*)str.data());
}

void ScStudio::SendCcdData(const uint8_t id,
		const array<uint16_t, Tsl1401cl::kSensorW> &ccd_data)
{
	Byte data[129];
	data[0] = id;
	for (Uint i = 0; i < ccd_data.size(); ++i)
	{
		data[i + 1] = ccd_data[i];
	}
	SendRaw(MessageToken::kCcdData, 129, data);
}

void ScStudio::SendCamera(const Byte *data, const size_t size)
{
	assert(size == 80 * 60 / 8);
	SendRaw(MessageToken::kCamera, size, data);
}

void ScStudio::SendRaw(const MessageToken token, const uint32_t size,
		const Byte *data)
{
	// zz zzzz zzyy yyyy yxxx xxxx => 1xxx xxxx 1yyy yyyy zzzz zzzz
	const size_t size_count = ((size >> 7) ? ((size >> 14) ? 3 : 2) : 1);
	vector<Byte> send(size + 3 + size_count);
	send[0] = kBegin;
	send[1] = static_cast<uint8_t>(token);
	int pos = 1;
	for (int i = 0; i < 3; ++i)
	{
		send[++pos] = size >> (7 * i);
		if (!(size >> (7 * (i + 1))))
		{
			break;
		}
		else if (i < 2)
		{
			send[pos] |= 0x80;
		}
	}
	memcpy(&send[++pos], data, size);
	send[send.size() - 1] = kEnd;

	if (m_uart)
	{
		m_uart->SendBuffer(std::move(send));
	}
}

bool ScStudio::OnRx(const Byte *data, const size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		if (!m_is_start && data[i] != kBegin)
		{
			continue;
		}
		else if (!m_is_start)
		{
			// b == kBegin
			m_is_start = true;
		}
		else if (m_token == MessageToken::kNull)
		{
			if (data[i] < static_cast<Uint>(MessageToken::kSize))
			{
				m_token = static_cast<MessageToken>(data[i]);
			}
			else
			{
				LOG_EL("Unknown msg token");
				Reset();
			}
		}
		else if (m_size == -1)
		{
			m_size = data[i];
		}
		else if (m_buf_it < m_size)
		{
			m_buf[m_buf_it++] = data[i];
		}
		else
		{
			if (data[i] == kEnd)
			{
				OnNewMessage();
			}
			else
			{
				LOG_EL("End byte mismatch");
				Reset();
			}
		}
	}
	return true;
}

void ScStudio::OnNewMessage()
{
	// :)
}

void ScStudio::Reset()
{
	m_is_start = false;
	m_token = MessageToken::kNull;
	m_size = -1;
	m_buf_it = 0;
}

}
