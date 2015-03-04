/*
 * uart_device.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "libbase/log.h"
#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/uart.h"

#include "libsc/config.h"
#include "libsc/kl26/uart_device.h"
#include "libutil/dynamic_block_buffer.h"
#include "libutil/misc.h"

using namespace libbase::kl26;
using namespace libutil;
using namespace std;

#define RX_BUFFER_SIZE 128

namespace libsc
{
namespace kl26
{

UartDevice::~UartDevice() {}
void UartDevice::SendStr(const char*) {}
void UartDevice::SendStr(unique_ptr<char[]>&&) {}
void UartDevice::SendStr(string&&) {}
void UartDevice::SendBuffer(const Byte*, const size_t) {}
void UartDevice::SendBuffer(unique_ptr<Byte[]>&&, const size_t) {}
void UartDevice::SendBuffer(vector<Byte>&&) {}
void UartDevice::EnableRx(const OnReceiveListener&) {}
void UartDevice::DisableRx() {}
bool UartDevice::PeekChar(char*) { return false; }

}
}
