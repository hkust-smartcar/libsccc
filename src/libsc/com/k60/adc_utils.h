/*
 * adc_utils.h
 * Utilities specified for K60
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_ADC_UTILS_H_
#define LIBSC_ADC_UTILS_H_

#include <MK60_adc.h>
#include <MK60_port.h>

namespace libsc
{

class AdcUtils
{
public:
	template<PTXn_e n>
	static inline ADCn_Ch_e GetAdcChannel();
};

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTB0>()
{
	return ADC0_SE8;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTB1>()
{
	return ADC0_SE9;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTA7>()
{
	return ADC0_SE10;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTA8>()
{
	return ADC0_SE11;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTB2>()
{
	return ADC0_SE12;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTB3>()
{
	return ADC0_SE13;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTC0>()
{
	return ADC0_SE14;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTC1>()
{
	return ADC0_SE15;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTE24>()
{
	return ADC0_SE17;
}

template<>
inline ADCn_Ch_e AdcUtils::GetAdcChannel<PTE25>()
{
	return ADC0_SE18;
}

}

#endif /* LIBSC_ADC_UTILS_H_ */
