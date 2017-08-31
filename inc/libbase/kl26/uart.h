/*
 * uart.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <bitset>
#include <functional>
#include <vector>

#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pin.h"

namespace libbase
{
namespace kl26
{

class Uart
{
public:
	typedef std::function<void(Uart *uart)> OnRxFullListener;
	typedef std::function<void(Uart *uart)> OnTxEmptyListener;

	enum struct Name
	{
		kUart0Rx = 0,
		kUart0Tx,
		kUart1Rx,
		kUart1Tx,
		kUart2Rx,
		kUart2Tx,

		kDisable
	};

	struct Config
	{
		enum struct BaudRate
		{
			k2400,
			k4800,
			k9600,
			k14400,
			k19200,
			k28800,
			k38400,
			k57600,
			k76800,
			k115200,
			k230400,
			k460800,
		};

		enum ConfigBit
		{
			/// Connect TX to RX
			kLoopMode = 0,
			/// Send 2 stop bits instead of 1
			kTwoStopBit,
			/// Enable parity bit or not, if enabled, it would be even parity
			kEnableEvenParity,

			kSize,
		};

		Pin::Name rx_pin;
		std::bitset<Pin::Config::ConfigBit::kSize> rx_config;
		Pin::Name tx_pin;
		std::bitset<Pin::Config::ConfigBit::kSize> tx_config;
		BaudRate baud_rate;
		std::bitset<ConfigBit::kSize> config;

		OnRxFullListener rx_isr;
		OnTxEmptyListener tx_isr;
	};

	explicit Uart(const Config &config);
	explicit Uart(nullptr_t);
	Uart(const Uart&) = delete;
	Uart(Uart &&rhs);
	~Uart();

	Uart& operator=(const Uart&) = delete;
	Uart& operator=(Uart &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void SetLoopMode(const bool flag);

	Byte GetByte() const;
	bool PeekByte(Byte *out_byte) const;
	void SendByte(const Byte byte);
	bool PutByte(const Byte byte);

	/**
	 * Enable Rx/Tx interrupt, by default they are both disabled after
	 * initialization and required programmer to explicitly enable them
	 *
	 * @param flag
	 */
	void SetEnableRxIrq(const bool flag);
	void SetEnableTxIrq(const bool flag);

private:
	bool InitModule(const Pin::Name rx_pin, const Pin::Name tx_pin);
	void InitPin(const Config &config);
	void InitBdhReg(const Config &config);
	void InitBaudRate(const Config::BaudRate br);
	void InitC1Reg(const Config &config);
	void InitUart0C4Reg();
	void InitInterrupt(const Config &config);

	void Uninit();

	void SetInterrupt(const bool rx_flag, const bool tx_flag);

	static __ISR void IrqHandler();

	uint8_t m_module;
	OnRxFullListener m_rx_isr;
	OnTxEmptyListener m_tx_isr;

	Pin m_rx;
	Pin m_tx;

	bool m_is_init;
};

}
}
