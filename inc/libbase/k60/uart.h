/*
 * uart.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <bitset>
#include <functional>
#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class Uart
{
public:
	typedef std::function<void(Uart *uart)> OnTxEmptyListener;
	typedef std::function<void(Uart *uart)> OnRxFullListener;

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
			kLoopMode,
			/// Enable parity bit or not, if enabled, it would be even parity
			kEnableEvenParity,
			kFifo,

			kSize,
		};

		Pin::Name tx_pin;
		std::bitset<Pin::Config::ConfigBit::kSize> tx_config;
		Pin::Name rx_pin;
		std::bitset<Pin::Config::ConfigBit::kSize> rx_config;
		BaudRate baud_rate;
		std::bitset<ConfigBit::kSize> config;

		OnTxEmptyListener tx_isr;
		/// The # bytes in the Tx buffer needed to fire the interrupt
		uint8_t tx_irq_threshold = 0;
		/// To treat tx_irq_threshold as a percentage of Tx buffer size
		bool is_tx_irq_threshold_percentage = false;

		OnRxFullListener rx_isr;
		/// The # bytes in the Tx buffer needed to fire the interrupt
		uint8_t rx_irq_threshold = 1;
		/// To treat rx_irq_threshold as a percentage of Rx buffer size
		bool is_rx_irq_threshold_percentage = false;
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

	uint8_t GetAvailableBytes() const;
	Byte GetByte() const;
	bool PeekByte(Byte *out_byte) const;
	std::vector<Byte> GetBytes() const;
	bool PeekBytes(std::vector<Byte> *out_bytes) const;
	void SendByte(const Byte byte);
	bool PutByte(const Byte byte);
	size_t PutBytes(const Byte *bytes, const size_t size);
	size_t PutBytes(const std::vector<Byte> &bytes)
	{
		return PutBytes(bytes.data(), bytes.size());
	}

	uint8_t GetTxFifoSize() const
	{
		return m_tx_fifo_size;
	}

	uint8_t GetRxFifoSize() const
	{
		return m_rx_fifo_size;
	}

	void SetEnableTxIrq(const bool flag);
	void SetEnableRxIrq(const bool flag);

private:
	enum Module
	{
		kUart0 = 0,
		kUart1,
		kUart2,
		kUart3,
		kUart4,
		kUart5,
	};

	bool InitModule(const Pin::Name tx_pin, const Pin::Name rx_pin);
	void InitBaudRate(const Config::BaudRate br);
	void InitPin(const Config &config);
	void InitC1Reg(const Config &config);
	void InitFifo(const Config &config);
	void InitInterrupt(const Config &config);

	void Uninit();

	void SetInterrupt(const bool tx_flag, const bool rx_flag);

	static __ISR void IrqHandler();

	Module m_module;
	bool m_is_fifo;

	Pin m_tx;
	uint8_t m_tx_fifo_size;
	OnTxEmptyListener m_tx_isr;

	Pin m_rx;
	uint8_t m_rx_fifo_size;
	OnRxFullListener m_rx_isr;

	bool m_is_init;
};

}
}
