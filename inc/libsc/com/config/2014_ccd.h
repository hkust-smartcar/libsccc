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
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_ENCODER_FTM
//#define LIBSC_USE_GYROSCOPE 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_LIGHT_SENSOR 2
#define LIBSC_USE_LINEAR_CCD 2
#define LIBSC_NEGATE_LINEAR_CCD
#define LIBSC_USE_MOTOR 1
#define LIBSC_NEGATE_MOTOR
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_SWITCH 5
#define LIBSC_USE_UART 1

#define LIBSC_BUTTON0 libbase::k60::PinConfig::Name::PTC11
#define LIBSC_BUTTON1 libbase::k60::PinConfig::Name::PTC10

#define LIBSC_BUZZER0 libbase::k60::PinConfig::Name::PTD5

#define LIBSC_ENCODER0_QDA PTA8
#define LIBSC_ENCODER0_QDB PTA9
#define LIBSC_ENCODER1_QDA PTB18
#define LIBSC_ENCODER1_QDB PTB19

#define LIBSC_GYROSCOPE_SCL PTB0
#define LIBSC_GYROSCOPE_SDA PTB1

#define LIBSC_JOYSTICK0_UP libbase::k60::PinConfig::Name::PTC6
#define LIBSC_JOYSTICK0_DOWN libbase::k60::PinConfig::Name::PTC5
#define LIBSC_JOYSTICK0_LEFT libbase::k60::PinConfig::Name::PTC8
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::PinConfig::Name::PTC7
#define LIBSC_JOYSTICK0_SELECT libbase::k60::PinConfig::Name::PTC9

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

#define LIBSC_LED0 libbase::k60::PinConfig::Name::PTD0
#define LIBSC_LED1 libbase::k60::PinConfig::Name::PTD1
#define LIBSC_LED2 libbase::k60::PinConfig::Name::PTD2
#define LIBSC_LED3 libbase::k60::PinConfig::Name::PTD3

#define LIBSC_LINEAR_CCD0_SI libbase::k60::PinConfig::Name::PTC1
#define LIBSC_LINEAR_CCD0_CLK libbase::k60::PinConfig::Name::PTC2
#define LIBSC_LINEAR_CCD0_AD libbase::k60::PinConfig::Name::PTB20
#define LIBSC_LINEAR_CCD1_SI libbase::k60::PinConfig::Name::PTC3
#define LIBSC_LINEAR_CCD1_CLK libbase::k60::PinConfig::Name::PTC4
#define LIBSC_LINEAR_CCD1_AD libbase::k60::PinConfig::Name::PTB21

#define LIBSC_LIGHT_SENSOR0 libbase::k60::PinConfig::Name::PTE25
#define LIBSC_LIGHT_SENSOR1 libbase::k60::PinConfig::Name::PTB22

#define LIBSC_MOTOR0_PWM libbase::k60::PinConfig::Name::PTD6
#define LIBSC_MOTOR0_DIR libbase::k60::PinConfig::Name::PTE6
#define LIBSC_MOTOR1_PWM libbase::k60::PinConfig::Name::PTD7
#define LIBSC_MOTOR1_DIR libbase::k60::PinConfig::Name::PTE7

#define LIBSC_SERVO0 libbase::k60::PinConfig::Name::PTA10

#define LIBSC_SYSTEM_TIMER_PIT_CHANNEL 0

#define LIBSC_SWITCH0 libbase::k60::PinConfig::Name::PTE8
#define LIBSC_SWITCH1 libbase::k60::PinConfig::Name::PTE9
#define LIBSC_SWITCH2 libbase::k60::PinConfig::Name::PTE10
#define LIBSC_SWITCH3 libbase::k60::PinConfig::Name::PTE11
#define LIBSC_SWITCH4 libbase::k60::PinConfig::Name::PTE12

#define LIBSC_UART0_TX libbase::k60::PinConfig::Name::PTC17
#define LIBSC_UART0_RX libbase::k60::PinConfig::Name::PTC16

#endif /* LIBSC_2014_CCD_H_ */
