/*
 * st7735r.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define ST7735R_SWRESET 0x01 // Software Reset
#define ST7735R_RDDID 0x04 // Read Display ID
#define ST7735R_SLPIN 0x10 // Sleep In
#define ST7735R_SLPOUT 0x11 // Sleep Out
#define ST7735R_INVOFF 0x20 // Display Inversion Off
#define ST7735R_INVON 0x21 // Display Inversion On
#define ST7735R_DISPOFF 0x28 // Display Off
#define ST7735R_DISPON 0x29 // Display On
#define ST7735R_CASET 0x2A // Column Address Set
#define ST7735R_RASET 0x2B // Row Address Set
#define ST7735R_RAMWR 0x2C // Memory Write
#define ST7735R_MADCTL 0x36 // Memory Data Access Control
#define ST7735R_COLMOD 0x3A // Interface Pixel Format
#define ST7735R_FRMCTRL1 0xB1 // Frame Rate Control (in normal mode)
#define ST7735R_INVCTR 0xB4 // Display Inversion Control
#define ST7735R_PWCTR1 0xC0 // Power Control 1
#define ST7735R_PWCTR2 0xC1 // Power Control 2
#define ST7735R_PWCTR3 0xC2 // Power Control 3 (in normal mode)
#define ST7735R_PWCTR4 0xC3 // Power Control 4 (in idle/8-bit mode)
#define ST7735R_PWCTR5 0xC4 // Power Control 5 (in partial mode)
#define ST7735R_VMCTR1 0xC5 // VCOM Control 1
#define ST7735R_GMCTRP1 0xE0 // Gamma (+ polarity) Correction Characteristics Setting
#define ST7735R_GMCTRN1 0xE1 // Gamma (- polarity) Correction Characteristics Setting
