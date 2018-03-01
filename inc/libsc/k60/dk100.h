/*
 * dk100.h
 * NFC reader
 *
 * Author: Dipsy Wong
 * Copyright (c) 2017-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include <functional>

#include "libsc/system.h"
#include "libbase/k60/uart.h"
#include "libsc/k60/uart_device.h"

using std::vector;
using libsc::System;

namespace libsc
{
namespace k60
{

class Dk100 : public UartDevice
{
public:
	typedef UartDevice::Config Config;
	typedef std::function<void(const Byte& sector)> OnWriteHandler;
	typedef std::function<void(const Byte& sector, const Byte *data)> OnReadHandler;

	explicit Dk100(const Config &config);
	explicit Dk100(nullptr_t);

	/**
	 * Make this the isr of dk100
	 */
	bool Listener(const Byte* data, const size_t size);

	/**
	 * Write 4 bytes to nfc card, halt the program until successfully send write command
	 * return false when operation is canceled
	 */
	bool SendWrite(const Byte& sector, const Byte *data);

	/**
	 * Send read command, halt the program until successfully send read command
	 * return false when operation is canceled
	 */
	bool SendRead(const Byte& sector);

	/**
	 * Send read command, halt the program until successfully send read command
	 * return false when operation is canceled, write result to target
	 */
	bool SendRead(const Byte& sector, Byte* target);

	/**
	 * return the last data read by send read command
	 */
	Byte* GetData() { return data; }

	/**
	 * Cancel the current sending process
	 */
	void Cancel();

	/**
	 * set the handler which will be called when successfully write data
	 */
	void SetWriteSuccessHandler(OnWriteHandler on_write_handler){ OnWrite = on_write_handler; }

	/**
	 * set the handler which will be called when successfully read data
	 */
	void SetReadSuccessHandler(OnReadHandler on_read_handler){ OnRead = on_read_handler; }

	//some byteconst for reference
	struct ByteConst{
		static const Byte 	kStartPkg = 0xAA;

		//general commands
		static const Byte 	kCardId = 0x01,
					kCardType = 0x02,
					kAutoSearch = 0x95,
					kModuleVersion = 0xB0;

		//Mifare card control (M1)
		struct M1{
		static const Byte 	kGiveKeyA = 0x03,
					kRead = 0x04,
					kWrite = 0x05,
					kValInit = 0x06,
					kIncrement = 0x07,
					kDecrement = 0x08,
					kGiveKeyB = 0x0B,
					kSetKeyType = 0x0C;
		};

		//Ultralight card control
		struct Ul{
			static const Byte	kRead = 0x09,
					kWrite = 0x0A;
		};

		struct ISO14443A{
			static const Byte	kActivate = 0x15,
					kAPDU = 0x17,
					kShutDown = 0x18;
		};

		struct IdCard{
			static const Byte	kActivate = 0x14,
					kAPDU = 0x16,
					kShutDown = 0x18;
		};

		struct ISO15693{
			static const Byte	kReadSingle = 0x90,
					kReadMultiple = 0x91,
					kWriteSingle = 0x92,
					kLock = 0x94;
		};

		struct Sam{
			static const Byte	kActivate = 0x19,
					kPDU = 0x1A,
					kDeactivate = 0x1B;
		};

		struct Ack{
			static const Byte	kCardTypeError = 0xE0,
					kCard404Error = 0xE1,
					kWrongKey = 0xE2,
					kReadFail = 0xE3,
					kWriteFail = 0xE4,
					kACK = 0xFE,
					kNACK = 0xFF;
			struct M1{
				static const Byte	kInitFail = 0xE5,
						kIncreFail = 0xE6,
						kDecreFail = 0xE7;
			};
		};
	};

protected:
	struct Initializer : public UartDevice::Initializer
	{
		explicit Initializer(const Config &config)
				: UartDevice::Initializer(config)
		{}

		libbase::k60::Uart::Config GetUartConfig() const override;
	};
private:
	OnWriteHandler OnWrite = nullptr;
	OnReadHandler OnRead = nullptr;
	bool waiting_write = false;
	bool waiting_read = false;
	Byte sending_buffer[8];
	Byte data[4];
	vector<Byte> buffer;
	bool cancel = false;

	/**
	 * called when a complete package is received
	 */
	void Handler(const vector<Byte>& v);
};


}
}
