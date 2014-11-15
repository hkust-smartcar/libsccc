/*
 * vcan_fx15dev.h
 * Config file for the VCAN K60FX15 development board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BUTTON 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_UART 1

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtd7

#define LIBSC_LCD_RST libbase::k60::Pin::Name::kPta13
#define LIBSC_LCD_DC libbase::k60::Pin::Name::kPta17
#define LIBSC_LCD_CS libbase::k60::Pin::Name::kPta14
#define LIBSC_LCD_SDAT libbase::k60::Pin::Name::kPta16
#define LIBSC_LCD_SCLK libbase::k60::Pin::Name::kPta15

#define LIBSC_LED0 libbase::k60::Pin::Name::kPtb20
#define LIBSC_LED1 libbase::k60::Pin::Name::kPtb21
#define LIBSC_LED2 libbase::k60::Pin::Name::kPtb22
#define LIBSC_LED3 libbase::k60::Pin::Name::kPtb23

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtd7
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtd6
