/*
 * 2014_ccd.h
 * Config file for 2014 CCD board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_BUTTON 2
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_ENCODER 1
//#define LIBSC_USE_SOFT_ENCODER 1
#define LIBSC_USE_GYROSCOPE 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_LIGHT_SENSOR 2
#define LIBSC_USE_LINEAR_CCD 2
#define LIBSC_NEGATE_LINEAR_CCD
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
//#define LIBSC_USE_SOFT_SERVO_PWM 1
#define LIBSC_USE_SWITCH 5
#define LIBSC_USE_UART 1

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtc11
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPtc10

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPtd5

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta8
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta9
#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPtb18
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPtb19

//#define LIBSC_GYROSCOPE_SCL PTB0
//#define LIBSC_GYROSCOPE_SDA PTB1

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc6
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtc5
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtc8
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtc7
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtc9

#define LIBSC_LCD_RST libbase::k60::Pin::Name::kPta13
#define LIBSC_LCD_DC libbase::k60::Pin::Name::kPta17
#define LIBSC_LCD_CS libbase::k60::Pin::Name::kPta14
#define LIBSC_LCD_SDAT libbase::k60::Pin::Name::kPta16
#define LIBSC_LCD_SCLK libbase::k60::Pin::Name::kPta15
#define LIBSC_ST7735R_BGR_PANEL 1

#define LIBSC_LED0 libbase::k60::Pin::Name::kPtd0
#define LIBSC_LED1 libbase::k60::Pin::Name::kPtd1
#define LIBSC_LED2 libbase::k60::Pin::Name::kPtd2
#define LIBSC_LED3 libbase::k60::Pin::Name::kPtd3

#define LIBSC_LINEAR_CCD0_SI libbase::k60::Pin::Name::kPtc1
#define LIBSC_LINEAR_CCD0_CLK libbase::k60::Pin::Name::kPtc2
#define LIBSC_LINEAR_CCD0_AD libbase::k60::Pin::Name::kPtb20
#define LIBSC_LINEAR_CCD1_SI libbase::k60::Pin::Name::kPtc3
#define LIBSC_LINEAR_CCD1_CLK libbase::k60::Pin::Name::kPtc4
#define LIBSC_LINEAR_CCD1_AD libbase::k60::Pin::Name::kPtb21

#define LIBSC_LIGHT_SENSOR0 libbase::k60::Pin::Name::kPte25
#define LIBSC_LIGHT_SENSOR1 libbase::k60::Pin::Name::kPtb22

#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtd6
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPte6
#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtd7
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPte7

#define LIBSC_DIR_MOTOR_CW_LEVEL 0

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPta10
#define LIBSC_SERVO0_SOFT_PWM_PIT_CHANNEL 1

#define LIBSC_SYSTEM_TIMER_PIT_CHANNEL 0

#define LIBSC_SWITCH0 libbase::k60::Pin::Name::kPte8
#define LIBSC_SWITCH1 libbase::k60::Pin::Name::kPte9
#define LIBSC_SWITCH2 libbase::k60::Pin::Name::kPte10
#define LIBSC_SWITCH3 libbase::k60::Pin::Name::kPte11
#define LIBSC_SWITCH4 libbase::k60::Pin::Name::kPte12

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtc17
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtc16
