/*
 * passive_buzzer.h
 *
 * Author: Kyle Lei
 * Copyright (c) 2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <map>

#include "libbase/helper.h"
#include "libbase/pinout_macros.h"
#include LIBBASE_H(soft_pwm)

namespace libsc {

/**
 * add "#define LIBSC_PASSSIVE_BUZZER0_PIT_CHANNEL 1" && "#define LIBSC_USE_PASSIVE_BUZZER 1" to config file
 * to use passive buzzer
 */
class PassiveBuzzer {
public:

	typedef LIBBASE_MODULE(SoftPwm) Pwm;//used SoftPwm since its freq can be adjusted at any time

	struct Config {
		uint32_t freq = 440;
		uint32_t pos = 0;
		uint8_t id = 0;
	};

	explicit PassiveBuzzer(const Config &config);

	//note in Hz, refer to libutil/notes.h for frequencies
	void SetNote(const uint32_t note);
	//duty_cycle like the setpower for motors, [0,1000], the sonic texture of the sound will change
	//note that when set to 1000, the passive buzzer won't respond since the input is a constant voltage
	void SetTexture(const uint32_t duty_cycle);
	//set note and texture at the same time
	void Set(const uint32_t note,const uint32_t duty_cycle);

	//these two are identical with the simple buzzer
	void SetBeep(const bool is_beep);

	bool GetBeep() const {return (bool) m_duty_cycle;};

private:
	Pwm m_pwm;

	uint32_t m_note;
	uint32_t m_duty_cycle;

};

}
