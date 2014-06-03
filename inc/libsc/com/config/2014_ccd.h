/*
 * 2013_ccd.h
 * Config file for 2014 CCD board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_2014_CCD_H_
#define LIBSC_2014_CCD_H_

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_BUTTON 2
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_ENCODER_FTM
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_LINEAR_CCD 2
#define LIBSC_NEGATE_LINEAR_CCD
#define LIBSC_USE_MOTOR 1
#define LIBSC_NEGATE_MOTOR
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_SWITCH 4
#define LIBSC_USE_UART 1

#define LIBSC_BUTTON0 PTC11
#define LIBSC_BUTTON1 PTC10

#define LIBSC_ENCODER0_QDA PTA8
#define LIBSC_ENCODER0_QDB PTA9

#ifdef LIBSC_USE_LCD_HW_SPI
#define LIBSC_LCD_RST PTA13
	#define LIBSC_LCD_DC PTA17
	#define LIBSC_LCD_CS PTA14
	// Connect SOUT pin with SDAT
	#define LIBSC_LCD_MOSI PTA16
	// MISO pin is not used (one way transfer) but is needed to get the SPI info
	#define LIBSC_LCD_MISO PTA17
	#define LIBSC_LCD_SCLK PTA15
#else
	#define LIBSC_LCD_RST PTA13
	#define LIBSC_LCD_DC PTA17
	#define LIBSC_LCD_CS PTA14
	#define LIBSC_LCD_SDAT PTA16
	#define LIBSC_LCD_SCLK PTA15
#endif

#define LIBSC_JOYSTICK_UP PTC5
#define LIBSC_JOYSTICK_DOWN PTC6
#define LIBSC_JOYSTICK_LEFT PTC7
#define LIBSC_JOYSTICK_RIGHT PTC8
#define LIBSC_JOYSTICK_SELECT PTC9

#define LIBSC_LED0 PTD0
#define LIBSC_LED1 PTD1
#define LIBSC_LED2 PTD2
#define LIBSC_LED3 PTD3

#define LIBSC_LINEAR_CCD0_SI PTC1
#define LIBSC_LINEAR_CCD0_CLK PTC2
#define LIBSC_LINEAR_CCD0_AO PTB20
#define LIBSC_LINEAR_CCD1_SI PTC3
#define LIBSC_LINEAR_CCD1_CLK PTC4
#define LIBSC_LINEAR_CCD1_AO PTB21

#define LIBSC_MOTOR0_PWM PTD6
#define LIBSC_MOTOR0_DIR PTE6

#define LIBSC_SERVO0 PTA10

#define LIBSC_SWITCH0 PTE8
#define LIBSC_SWITCH1 PTE9
#define LIBSC_SWITCH2 PTE10
#define LIBSC_SWITCH3 PTE11
#define LIBSC_SWITCH4 PTE12

#endif /* LIBSC_2014_CCD_H_ */
