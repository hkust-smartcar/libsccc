/*
 * ftm_pwm.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>

#include <cassert>
#include <cstdint>

#include <MK60_FTM.h>

#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

namespace
{

inline uint32_t GetFrequency(const uint32_t period)
{
	return 1000000 / period;
}

inline uint32_t GetDuty(const uint32_t period, const uint32_t high_time)
{
	return high_time * 10000 / period;
}

}

FtmPwm::FtmPwm(const Config &config)
		: m_period_us(config.period_us)
{
	if (!InitModule(config.pin))
	{
		assert(false);
	}
	FTM_PWM_init((FTMn_e)m_module, (FTM_CHn_e)m_channel,
			GetFrequency(config.period_us),
			GetDuty(config.period_us, config.high_time_us));
}

bool FtmPwm::InitModule(const PinConfig::Name pin)
{
	// Temporarily place here
	FTMn_e module = FTMn_e::FTM_MAX;
	FTM_CHn_e channel = FTM_CHn_e::FTM_CH0;
	switch (pin)
	{
	case PinConfig::Name::PTA3:
	case PinConfig::Name::PTC1:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH0;
		break;

	case PinConfig::Name::PTA4:
	case PinConfig::Name::PTC2:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH1;
		break;

	case PinConfig::Name::PTA5:
	case PinConfig::Name::PTC3:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH2;
		break;

	case PinConfig::Name::PTA6:
	case PinConfig::Name::PTC4:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH3;
		break;

	case PinConfig::Name::PTA7:
	case PinConfig::Name::PTD4:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH4;
		break;

	case PinConfig::Name::PTA0:
	case PinConfig::Name::PTD5:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH5;
		break;

	case PinConfig::Name::PTA1:
	case PinConfig::Name::PTD6:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH6;
		break;

	case PinConfig::Name::PTA2:
	case PinConfig::Name::PTD7:
		module = FTMn_e::FTM0;
		channel = FTM_CHn_e::FTM_CH7;
		break;

	case PinConfig::Name::PTA8:
	case PinConfig::Name::PTA12:
	case PinConfig::Name::PTB0:
		module = FTMn_e::FTM1;
		channel = FTM_CHn_e::FTM_CH0;
		break;

	case PinConfig::Name::PTA9:
	case PinConfig::Name::PTA13:
	case PinConfig::Name::PTB1:
		module = FTMn_e::FTM1;
		channel = FTM_CHn_e::FTM_CH1;
		break;

	case PinConfig::Name::PTA10:
	case PinConfig::Name::PTB18:
		module = FTMn_e::FTM2;
		channel = FTM_CHn_e::FTM_CH0;
		break;

	case PinConfig::Name::PTA11:
	case PinConfig::Name::PTB19:
		module = FTMn_e::FTM2;
		channel = FTM_CHn_e::FTM_CH1;
		break;

	default:
		break;
	}

	if (module != FTMn_e::FTM_MAX)
	{
		m_module = module;
		m_channel = channel;
		return true;
	}
	else
	{
		return false;
	}
}

void FtmPwm::SetPeriodUs(const uint32_t period_us, const uint32_t high_time_us)
{
	FTM_PWM_freq((FTMn_e)m_module, GetFrequency(period_us));
	FTM_PWM_Duty((FTMn_e)m_module, (FTM_CHn_e)m_channel, GetDuty(period_us,
			high_time_us));
}

void FtmPwm::SetHighTimeUs(const uint32_t high_time_us)
{
	FTM_PWM_Duty((FTMn_e)m_module, (FTM_CHn_e)m_channel, GetDuty(m_period_us,
			high_time_us));
}

}
}
