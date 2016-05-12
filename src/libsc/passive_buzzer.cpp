/*
 * passive_buzzer.cpp
 *
 * Author: Kyle Lei
 * Copyright (c) 2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(soft_pwm)

#include LIBBASE_H(pwm_utils)
#include "libsc/config.h"
#include "libsc/passive_buzzer.h"
#include "libutil/misc.h"

using namespace LIBBASE_NS;

namespace libsc {

#ifdef LIBSC_USE_PASSIVE_BUZZER

namespace {

#if LIBSC_USE_PASSIVE_BUZZER == 1
inline Pin::Name GetPin(const uint8_t id)
{
	assert(id==0);
	return LIBSC_BUZZER0;
}

inline uint8_t GetSoftPitChannel(const uint8_t)
{
	return LIBSC_PASSSIVE_BUZZER0_PIT_CHANNEL;
}

#endif


PassiveBuzzer::Pwm::Config GetPwmConfig(const uint8_t id, const uint32_t freq, const uint32_t pos) {
	PassiveBuzzer::Pwm::Config config;
	config.pin = GetPin(id);
	config.period = PwmUtils::GetPeriodUs(freq);
	config.pos_width = pos;
	config.pit_channel = GetSoftPitChannel(id);
	return config;
}

}

PassiveBuzzer::PassiveBuzzer(const Config &config) :
		m_pwm(GetPwmConfig(config.id,config.freq,config.pos)),m_note(config.freq), m_duty_cycle(config.pos) {
}

void PassiveBuzzer::SetNote(const uint32_t note) {

	if (note == m_note) return;

	m_note = note;
	m_pwm.SetPeriod(PwmUtils::GetPeriodUs(m_note),PwmUtils::GetPosWidth(PwmUtils::GetPeriodUs(m_note), m_duty_cycle));

}

void PassiveBuzzer::SetTexture(const uint32_t duty_cycle) {

	if (duty_cycle == m_duty_cycle) return;

	m_duty_cycle = libutil::Clamp<uint32_t>(0, duty_cycle, 1000);
	m_pwm.SetPosWidth(PwmUtils::GetPosWidth(PwmUtils::GetPeriodUs(m_note), m_duty_cycle));
}

void PassiveBuzzer::Set(const uint32_t note,const uint32_t duty_cycle){

	if (duty_cycle == m_duty_cycle || note == m_note) return;

	m_note = note;
	m_duty_cycle = libutil::Clamp<uint32_t>(0, duty_cycle, 1000);
	m_pwm.SetPeriod(PwmUtils::GetPeriodUs(m_note),PwmUtils::GetPosWidth(PwmUtils::GetPeriodUs(m_note), m_duty_cycle));
	m_pwm.SetPosWidth(PwmUtils::GetPosWidth(PwmUtils::GetPeriodUs(m_note), m_duty_cycle));
}

void PassiveBuzzer::SetBeep(const bool is_beep) {
	m_duty_cycle = is_beep * 500;
	m_pwm.SetPosWidth(PwmUtils::GetPosWidth(PwmUtils::GetPeriodUs(m_note), m_duty_cycle));
}
#else
PassiveBuzzer::PassiveBuzzer(const Config&) :
		m_pwm(nullptr), m_note(0), m_duty_cycle(0) {
	LOG_DL("Configured not to use PassiveBuzzer");
}

void PassiveBuzzer::SetNote(const uint32_t) {}
void PassiveBuzzer::SetTexture(const uint32_t) {}
void PassiveBuzzer::Set(const uint32_t,const uint32_t){}
void PassiveBuzzer::SetBeep(const bool){}

#endif /* LIBSC_USE_PASSIVE_BUZZER */
}
