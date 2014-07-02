/*
 * buzzer.h
 * Buzzer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_BUZZER_H_
#define LIBSC_BUZZER_H_

#include <cstdint>

namespace libsc
{

class Buzzer
{
public:
	explicit Buzzer(const uint8_t id);

	void SetBeep(const bool is_beep);

private:
	const uint8_t m_id;
};

}

#endif /* LIBSC_BUZZER_H_ */
