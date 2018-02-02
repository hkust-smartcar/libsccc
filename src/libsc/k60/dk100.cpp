/*
 * dk100.cpp
 * NFC reader
 *
 * Author: Dipsy Wong
 * Copyright (c) 2017-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libbase/k60/pin.h"
#include "libbase/k60/uart.h"

#include "libsc/config.h"
#include "libsc/k60/dk100.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_UART

Uart::Config Dk100::Initializer::GetUartConfig() const
{
	Uart::Config product = UartDevice::Initializer::GetUartConfig();
	// On this board, there's a diode connected to the Tx pin, preventing the
	// module to correctly send data to the MCU
	product.baud_rate = libbase::k60::Uart::Config::BaudRate::k9600;
	product.rx_config[Pin::Config::kPullEnable] = true;
	product.rx_config[Pin::Config::kPullUp] = true;
	return product;
}

Dk100::Dk100(const Config &config)
		: UartDevice(Initializer(config))
{
}

void Dk100::Handler(const vector<Byte>& v){
	//trigger when card tab if auto detect card is turned on
	if(v[2] == ByteConst::kCardId){
	}

	//trigger when write success
	if(waiting_write && v[2] == ByteConst::Ack::kACK){
		waiting_write = false;
		if(OnWrite) OnWrite(v[3]);
	}

	//trigger when read success
	if(v[2] == 0x90){
		waiting_read = false;
		if(OnRead){
			const Byte buf[4] = {v[4],v[5],v[6],v[7]};
			OnRead(v[3],buf);
		}
	}
}

bool Dk100::Listener(const Byte* data, const size_t size){
	for(uint8_t i=0; i<size; i++){
		buffer.push_back(data[i]);
		if(buffer.size() > 1){
			if(buffer.size() == buffer[1]+2){
				Handler(buffer);
				buffer.clear();
			} else if (buffer.size() > buffer[1]+2) {
				buffer.clear();
			}
		}
	}
	return true;
}

void Dk100::SendWriteHelper(){
	this->SendBuffer(sending_buffer,8);
}

void Dk100::SendWrite(const Byte& sector, const Byte *to_write){
	waiting_write = true;
	Byte buf[8] = {0xAA, 0x06, 0x0A, sector};
	memcpy(sending_buffer,buf,4);
	memcpy(sending_buffer+4,to_write,4);
	while(waiting_write)SendWriteHelper();
}

void Dk100::SendReadHelper(){
	this->SendBuffer(sending_buffer,4);
}
void Dk100::SendRead(const Byte& sector){
	waiting_read = true;
	Byte buf[4] = {0xAA, 0x02, 0x09, sector};
	memcpy(sending_buffer,buf,4);
	while(waiting_read)SendReadHelper();
}

#else /* LIBSC_USE_UART */
Dk100::Dk100(const Config&)
		: UartDevice(nullptr)
{}

Dk100::Dk100(nullptr_t)
:
	UartDevice(nullptr)
{}

#endif /* LIBSC_USE_UART */

}
}
