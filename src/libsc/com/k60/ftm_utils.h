/*
 * ftm_utils.h
 * Utilities specified for K60
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_FTM_UTILS_H_
#define LIBSC_FTM_UTILS_H_

#include <FIRE_PORT_cfg.h>
#include <MK60_FTM.h>
#include <MK60_port.h>

namespace libsc
{

class FtmUtils
{
public:
	/**
	 * Return the FTM module set for the specified GPIO pin @a n. This method
	 * will cause a link-time error if @a n is unknown
	 *
	 * @return
	 */
	template<PTXn_e n>
	static inline FTMn_e GetFtmModule();

	/**
	 * Return the FTM channel set for the specified GPIO pin @a n. This method
	 * will cause a link-time error if @a n is unknown
	 *
	 * @return
	 */
	template<PTXn_e n>
	static inline FTM_CHn_e GetFtmChannel();
};

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH0>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH1>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH2>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH3>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH4>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH5>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH6>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM0_CH7>()
{
	return FTM0;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM1_CH0>()
{
	return FTM1;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM1_CH1>()
{
	return FTM1;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM2_CH0>()
{
	return FTM2;
}

template<>
inline FTMn_e FtmUtils::GetFtmModule<FTM2_CH1>()
{
	return FTM2;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH0>()
{
	return FTM_CH0;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH1>()
{
	return FTM_CH1;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH2>()
{
	return FTM_CH2;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH3>()
{
	return FTM_CH3;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH4>()
{
	return FTM_CH4;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH5>()
{
	return FTM_CH5;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH6>()
{
	return FTM_CH6;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM0_CH7>()
{
	return FTM_CH7;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM1_CH0>()
{
	return FTM_CH0;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM1_CH1>()
{
	return FTM_CH1;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM2_CH0>()
{
	return FTM_CH0;
}

template<>
inline FTM_CHn_e FtmUtils::GetFtmChannel<FTM2_CH1>()
{
	return FTM_CH1;
}

}

#endif /* LIBSC_FTM_UTILS_H_ */
