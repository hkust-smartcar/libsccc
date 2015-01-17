/*
 * 2014_camera.h
 * Config file for 2014 Gen 1 board (camera car)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#ifndef LIBSC_2014_CAMERA_H_
#define LIBSC_2014_CAMERA_H_

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_BUTTON 1
//#define LIBSC_USE_CAMERA 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_TEMPERATURE_SENSOR 0
#define LIBSC_USE_UART 1

#define LIBSC_ACCELEROMETER_RX libbase::k60::Adc::Name::kAdc0Ad17

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc1Ad4B

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtc10

//#define LIBSC_CAMERA_SDA PTB17
//#define LIBSC_CAMERA_SCL PTB16
//#define LIBSC_CAMERA_PCLK PTB2
//#define LIBSC_CAMERA_HREF PTB7
//#define LIBSC_CAMERA_VSYNC PTB4
//#define LIBSC_CAMERA_DMA_SRC_ADDR PTD8
//#define LIBSC_CAMERA_DMA_CH 0

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPtb0
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPtb1
#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPtb18
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPtb19

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

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte9
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte10
#define LIBSC_LED2 libbase::k60::Pin::Name::kPte11
#define LIBSC_LED3 libbase::k60::Pin::Name::kPte12

#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtd7
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPte7
#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtd6
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPte6

#define LIBSC_DIR_MOTOR_CW_LEVEL 0

#define LIBSC_TEMPERATURE0 libbase::k60::Pin::Name::kPtc2

#endif /* LIBSC_2014_CAMERA_H_ */
