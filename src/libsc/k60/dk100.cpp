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
		if(OnWrite) OnWrite(v[3]);
		waiting_write = false;
	}

	//trigger when read success
	if(waiting_read && v[2] == ByteConst::Ul::kRead){
		memcpy(data,&v[4],4);
		if(OnRead){
//			const Byte buf[4] = {v[4],v[5],v[6],v[7]};
			OnRead(v[3],data);
		}
		waiting_read = false;
	}
}

bool Dk100::Listener(const Byte* data, const size_t size){
	for(uint8_t i=0; i<size; i++){
		if(data[i]==0xAA)buffer.clear();
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

bool Dk100::SendWrite(const Byte& sector, const Byte *to_write){
	data[0]=data[1]=data[2]=data[3]=0;
	cancel = false;
	waiting_write = true;
	Byte buf[8] = {ByteConst::kStartPkg, 0x06, ByteConst::Ul::kWrite, sector};
	memcpy(buf+4,to_write,4);
	while(waiting_write)SendBuffer(buf,8);
	return !cancel;
}

bool Dk100::SendRead(const Byte& sector){
	data[0]=data[1]=data[2]=data[3]=0;
	cancel = false;
	waiting_read = true;
	Byte buf[4] = {ByteConst::kStartPkg, 0x02, ByteConst::Ul::kRead, sector};
	while(waiting_read)SendBuffer(buf,4);
	System::DelayMs(100);
	return !cancel;
}

bool Dk100::SendRead(const Byte& sector, Byte* target){
	if(SendRead(sector)){
		memcpy(target,data,4);
		return true;
	}
	else{
		return false;
	}
}

void Dk100::Cancel(){
	cancel = true;
	waiting_read = false;
	waiting_write = false;
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
