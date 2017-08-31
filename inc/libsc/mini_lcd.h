/*
 * MiniLcd.h
 *
 * Author: petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <libbase/helper.h>
#include <libsc/battery_meter.h>
#include <libsc/lcd.h>
#include <libsc/timer.h>
#include <libsc/lcd_console.h>
#include <libsc/mini_typewriter.h>

#define MAX_ASCII_INDEX 126
#define MIN_ASCII_INDEX 32

#define inRange(n, v, x) ((v < n)? n : ((v > x)? x : v))


namespace libsc
{

const char endl = '\n';

class MiniLcd : private MiniTypeWriter
{

public:

	struct Config
	{
		uint16_t bgColor;
		uint16_t txtColor;
		uint8_t fps;

		uint8_t floatPrecision = 3;

		bool isTextWrap = false;
		bool isClearLine = false;

		Config(uint16_t _bgColor, uint16_t _txtColor, uint8_t _fps, uint8_t _floatPrecision = 3, bool _isTextWrap = false, bool _isClearLine = false) : bgColor(_bgColor), txtColor(_txtColor), fps(_fps), floatPrecision(_floatPrecision), isTextWrap(_isTextWrap), isClearLine(_isClearLine) {};
	};

	const static char endl = '\n';

	explicit MiniLcd(MiniLcd::Config config);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	MiniLcd &setRow(const uint8_t &row);
	MiniLcd &operator<<(const bool b);
	MiniLcd &operator<<(const char c);
	MiniLcd &operator<<(const char *str);
	MiniLcd &operator<<(const float &f);
	MiniLcd &operator<<(const uint8_t &ub);
	MiniLcd &operator<<(const uint16_t &us);
	MiniLcd &operator<<(const uint32_t &ui);
	MiniLcd &operator<<(const int8_t &b);
	MiniLcd &operator<<(const int16_t &s);
	MiniLcd &operator<<(const int32_t &i);

	void clear(void);

	St7735r					*m_lcd;

private:

	Timer::TimerInt			m_lastUpdateTime;

	LcdConsole::Config getLcdConsoleConfig(MiniLcd::Config MiniLcdconfig);
	MiniTypeWriter::Config getTypeWriterConfig(MiniLcd::Config MiniLcdconfig);

	static const bool BatteryCharge[];
	static const uint8_t BatteryOutlook[];

	bool					m_enabled;

	char 					m_setFloatPrecisionChar[15];

	static MiniLcd			*m_instance;

};

}
