/*
 * MiniLcd.cpp
 *
 * Author: petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cmath>

#include <libbase/helper.h>
#include <libsc/st7735r.h>
#include <libsc/device_h/st7735r.h>
#include <libsc/lcd_console.h>
#include <libbase/misc_utils_c.h>
#include <libsc/lcd.h>
#include <libsc/mini_lcd.h>

namespace libsc
{

MiniLcd *MiniLcd::m_instance = nullptr;

const uint8_t MiniLcd::BatteryOutlook[] =
{
	60, 255, 195, 195, 195, 195, 195,
	195, 195, 195, 195, 195, 255
};
const bool MiniLcd::BatteryCharge[4 * 10] = { 1 };

LcdConsole::Config MiniLcd::getLcdConsoleConfig(MiniLcd::Config MiniLcdconfig)
{
	St7735r::Config config;
	config.is_revert = false;
	config.is_bgr = true;
	config.fps = MiniLcdconfig.fps;
	LcdConsole::Config consoleConfig;
	m_lcd = new St7735r(config);
	consoleConfig.bg_color = MiniLcdconfig.bgColor;
	consoleConfig.text_color = MiniLcdconfig.txtColor;
	consoleConfig.lcd = m_lcd;
	return consoleConfig;
}

MiniTypeWriter::Config MiniLcd::getTypeWriterConfig(MiniLcd::Config MiniLcdconfig)
{
	MiniTypeWriter::Config config;
	config.bg_color = MiniLcdconfig.bgColor;
	config.text_color = MiniLcdconfig.txtColor;
	config.is_text_wrap = MiniLcdconfig.isTextWrap;
	config.is_clear_line = MiniLcdconfig.isClearLine;

	St7735r::Config lcdConfig;
	lcdConfig.is_revert = true;
	lcdConfig.is_bgr = true;
	lcdConfig.fps = MiniLcdconfig.fps;
	m_lcd = new St7735r(lcdConfig);

	config.lcd = m_lcd;
	return config;
}

MiniLcd::MiniLcd(MiniLcd::Config config)
:
	MiniTypeWriter(getTypeWriterConfig(config)),
	m_lastUpdateTime(0),
	m_enabled(true),
	m_setFloatPrecisionChar{ "%.03f\0" }
{
	if (!m_instance)
		m_instance = this;

	if (config.floatPrecision >= 10)
		m_setFloatPrecisionChar[2] = '0' + (config.floatPrecision / 10);
	m_setFloatPrecisionChar[3] = '0' + (config.floatPrecision % 10);
}

void MiniLcd::setEnabled(const bool enabled)
{
	m_enabled = enabled;
}

bool MiniLcd::isEnabled(void)
{
	return m_enabled;
}

MiniLcd &MiniLcd::setRow(const uint8_t &row)
{
	if (m_enabled)
		SetCursorRow(row);
	return *this;
}

MiniLcd &MiniLcd::operator<<(const bool b)
{
	if (m_enabled)
		if (b)
			WriteString("true");
		else
			WriteString("false");
	return *this;
}

MiniLcd &MiniLcd::operator<<(const char c)
{
	if (m_enabled)
		WriteChar(c);
	return *this;
}

MiniLcd &MiniLcd::operator<<(const char *str)
{
	if (m_enabled)
		WriteString(str);
	return *this;
}

MiniLcd &MiniLcd::operator<<(const float &f)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, m_setFloatPrecisionChar, f);
		WriteBuffer(buffer, n);
	}
	return *this;
}

MiniLcd &MiniLcd::operator<<(const uint8_t &ub)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, "%d", ub);
		WriteBuffer(buffer, n);
	}
	return *this;
}

MiniLcd &MiniLcd::operator<<(const uint16_t &us)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, "%d", us);
		WriteBuffer(buffer, n);
	}
	return *this;
}

MiniLcd &MiniLcd::operator<<(const uint32_t &ui)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, "%ld", ui);
		WriteBuffer(buffer, n);
	}
	return *this;
}

MiniLcd &MiniLcd::operator<<(const int8_t &b)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, "%d", b);
		WriteBuffer(buffer, n);
	}
	return *this;
}

MiniLcd &MiniLcd::operator<<(const int16_t &s)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, "%d", s);
		WriteBuffer(buffer, n);
	}
	return *this;
}

MiniLcd &MiniLcd::operator<<(const int32_t &i)
{
	if (m_enabled)
	{
		char buffer[20] = { 0 };
		size_t n = sprintf(buffer, "%ld", i);
		WriteBuffer(buffer, n);
	}
	return *this;
}

void MiniLcd::clear(void)
{
	SetCursorRow(0);
	m_lcd->Clear();
}

}
