/*
 * dma_mux.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pinout_macros.h"

namespace libbase
{
namespace k60
{

class DmaMux
{
public:
	enum struct Source
	{
		/// Only enable the channel through software (i.e., Dma::Start())
		kNull = 0,
		kAdc0,
		kAdc1,
#if PINOUT_ADC_COUNT > 2
		kAdc2,
		kAdc3,
#endif
//		kCmp0,
//		kCmp1,
//		kCmp2,
//		kCmp3,
//		kCmt,
		kDac0,
		kDac1,
		kFtm0Ch0,
		kFtm0Ch1,
		kFtm0Ch2,
		kFtm0Ch3,
		kFtm0Ch4,
		kFtm0Ch5,
		kFtm0Ch6,
		kFtm0Ch7,
		kFtm1Ch0,
		kFtm1Ch1,
		kFtm2Ch0,
		kFtm2Ch1,
#if PINOUT_FTM_COUNT > 3
		kFtm3Ch0,
		kFtm3Ch1,
		kFtm3Ch2,
		kFtm3Ch3,
		kFtm3Ch4,
		kFtm3Ch5,
		kFtm3Ch6,
		kFtm3Ch7,
#endif
		kI2c0,
		kI2c1or2,
//		kI2s0Rx,
//		kI2s0Tx,
//		kI2s1Rx,
//		kI2s1Tx,
//		kIeee1588Timer0,
//		kIeee1588Timer1,
//		kIeee1588Timer2,
//		kIeee1588Timer3,
//		kPdb,
		kPortA,
		kPortB,
		kPortC,
		kPortD,
		kPortE,
		kPortF,
		kSoftware,
		kSpi0Rx,
		kSpi0Tx,
		kSpi1Rx,
		kSpi1Tx,
#if PINOUT_SPI_COUNT > 2
		kSpi2Rx,
		kSpi2Tx,
#endif
		kUart0Rx,
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
	};

	static bool SetEnableSource(const Source src, const Uint channel);
};

}
}
