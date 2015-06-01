/*
 * 2015_ccd(2).h
 *
 *  Created on: 1 Jun, 2015
 *      Author: lincoln
 */

#pragma once

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUTTON 2
//#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
//#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 1
#define LIBSC_USE_LINEAR_CCD 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_MMA8451Q == 1
//#define LIBSC_USE_SOFT_MPU6050 1

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta8
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta9
#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPta10
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPta11

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPte3
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPte0
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPte4
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPte1
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPte2

#define LIBSC_ST7735R_BGR_PANEL 1


#define LIBSC_LINEAR_CCD1_SI libbase::k60::Pin::Name::kPta26
#define LIBSC_LINEAR_CCD1_CLK libbase::k60::Pin::Name::kPta28
#define LIBSC_LINEAR_CCD1_AD libbase::k60::Pin::Name::kPtb5
#define LIBSC_LINEAR_CCD2_SI libbase::k60::Pin::Name::kPta27
#define LIBSC_LINEAR_CCD2_CLK libbase::k60::Pin::Name::kPta29
#define LIBSC_LINEAR_CCD2_AD libbase::k60::Pin::Name::kPtb1
#define LIBSC_LINEAR_CCD3_SI libbase::k60::Pin::Name::kPta24
#define LIBSC_LINEAR_CCD3_CLK libbase::k60::Pin::Name::kPta25
#define LIBSC_LINEAR_CCD3_AD libbase::k60::Pin::Name::kPtb7

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kPtb10

//#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtc0
//#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPtb23

//#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta7
//#define LIBSC_BUZZER_ACIVE_LEVEL 0

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc16
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtd1
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtd6
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtd9
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtd14

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte12
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte11
//#define LIBSC_LED2 libbase::k60::Pin::Name::kPte10
//#define LIBSC_LED3 libbase::k60::Pin::Name::kPte9

#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtc9
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtc6
#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtc8
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPtc5

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPtc4
#define LIBSC_SERVO1 libbase::k60::Pin::Name::kPtc3


#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtb17
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtb16

#define LIBSC_MPU6050_SCL libbase::k60::Pin::Name::kPta17
#define LIBSC_MPU6050_SDA libbase::k60::Pin::Name::kPta16

#define LIBSC_MMA8451Q0_SCL libbase::k60::Pin::Name::kPta7
#define LIBSC_MMA8451Q0_SDA libbase::k60::Pin::Name::kPta6
