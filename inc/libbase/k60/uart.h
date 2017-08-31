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
#include <memory>
#include <vector>

#include "libbase/k60/dma.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
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
		kUart3Rx,
		kUart3Tx,
		kUart4Rx,
		kUart4Tx,
		kUart5Rx,
		kUart5Tx,

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
			kLoopMode,
			/// Enable parity bit or not, if enabled, it would be even parity
			kEnableEvenParity,
			kFifo,

			kSize,
		};

		Pin::Name rx_pin;
		std::bitset<Pin::Config::ConfigBit::kSize> rx_config;
		Pin::Name tx_pin;
		std::bitset<Pin::Config::ConfigBit::kSize> tx_config;
		BaudRate baud_rate;
		std::bitset<ConfigBit::kSize> config;

		OnRxFullListener rx_isr;
		/// The # bytes in the Rx buffer needed to fire the interrupt
		uint8_t rx_irq_threshold = 1;
		/// To treat rx_irq_threshold as a percentage of Rx buffer size
		bool is_rx_irq_threshold_percentage = false;

		OnTxEmptyListener tx_isr;
		/// The # bytes in the Tx buffer needed to fire the interrupt
		uint8_t tx_irq_threshold = 0;
		/// To treat tx_irq_threshold as a percentage of Tx buffer size
		bool is_tx_irq_threshold_percentage = false;
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
	/**
	 * Get 1 or more bytes. Will block until at least 1 byte is received
	 *
	 * @param out_size Return the size of the returned array
	 * @return Data array. The content of the array will only be hold until the
	 * next read call. The array will no longer be alive after the object is
	 * destructed
	 */
	const Byte* GetBytes(size_t *out_size) const;
	/**
	 * Peek 1 or more bytes. If no data is immediately available, nullptr would
	 * be returned instead
	 *
	 * @param out_size Return the size of the returned array, or 0 if nullptr is
	 * returned
	 * @return Data array. The content of the array will only be hold until the
	 * next read call. The array will no longer be alive after the object is
	 * destructed
	 */
	const Byte* PeekBytes(size_t *out_size) const;
	void SendByte(const Byte byte);
	bool PutByte(const Byte byte);
	size_t PutBytes(const Byte *bytes, const size_t size);
	size_t PutBytes(const std::vector<Byte> &bytes)
	{
		return PutBytes(bytes.data(), bytes.size());
	}

	uint8_t GetRxFifoSize() const
	{
		return m_rx_fifo_size;
	}

	uint8_t GetTxFifoSize() const
	{
		return m_tx_fifo_size;
	}

	/**
	 * Enable Tx/Rx interrupt, by default they are both disabled after
	 * initialization and require programmer to explicitly enable them
	 *
	 * @param flag
	 */
	void SetEnableRxIrq(const bool flag);
	void SetEnableTxIrq(const bool flag);

	/**
	 * Config this Uart up to be ready to serve Tx as DMA destination, and set
	 * @a config accordingly. The following options are also set besides mux_src
	 * and dst:<br>
	 * Dma::Config::minor_bytes = 1
	 *
	 * Tx IRQ will be disabled after invoking this method and needed to be
	 * enabled manually
	 *
	 * @note To use DMA, Config::tx_isr must NOT be set for this Uart. Otherwise,
	 * the operation will fail and no changes would be made to @a config
	 * @param config
	 */
	void ConfigTxAsDmaDst(Dma::Config *config);

private:
	bool InitModule(const Pin::Name rx_pin, const Pin::Name tx_pin);
	void InitPin(const Config &config);
	void InitBaudRate(const Config::BaudRate br);
	void InitC1Reg(const Config &config);
	void InitFifo(const Config &config);
	void InitInterrupt(const Config &config);

	void Uninit();

	void SetInterrupt(const bool rx_flag, const bool tx_flag);

	static __ISR void IrqHandler();

	uint8_t m_module;
	std::unique_ptr<Byte[]> m_rx_buf;
	OnRxFullListener m_rx_isr;
	OnTxEmptyListener m_tx_isr;

	bool m_is_fifo;
	uint8_t m_rx_fifo_size;
	uint8_t m_tx_fifo_size;

	Pin m_rx;
	Pin m_tx;

	bool m_is_init;
};

}
}
