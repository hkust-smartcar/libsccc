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
#define LIBSC_USE_MMA8451Q 1
//#define LIBSC_USE_TEMPERATURE_SENSOR 0
#define LIBSC_USE_UART 1
#define LIBSC_USE_LINEAR_CCD 1

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

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta8
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta9
#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPtb18
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPtb19

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc6
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtc5
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtc8
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtc7
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtc9

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPta13
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPta17
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPta14
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPta16
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPta15

#define LIBSC_LED0 libbase::k60::Pin::Name::kPtd0
#define LIBSC_LED1 libbase::k60::Pin::Name::kPtd1
#define LIBSC_LED2 libbase::k60::Pin::Name::kPtd2
#define LIBSC_LED3 libbase::k60::Pin::Name::kPtd3

#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtd7
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPte7
#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtd6
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPte6

#define LIBSC_DIR_MOTOR_CW_LEVEL 0

#define LIBSC_TEMPERATURE0 libbase::k60::Pin::Name::kPtc2

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtc17
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtc16

#define LIBSC_MPU6050_SCL libbase::k60::Pin::Name::kPtb22
#define LIBSC_MPU6050_SDA libbase::k60::Pin::Name::kPtb23

#define LIBSC_MMA8451Q0_SCL libbase::k60::Pin::Name::kPta12
#define LIBSC_MMA8451Q0_SDA libbase::k60::Pin::Name::kPtd4

#define LIBSC_LINEAR_CCD0_CLK libbase::k60::Pin::Name::kPta7
#define LIBSC_LINEAR_CCD0_SI libbase::k60::Pin::Name::kPtc0
#define LIBSC_LINEAR_CCD0_AD libbase::k60::Pin::Name::kPte0

#endif /* LIBSC_2014_CAMERA_H_ */
