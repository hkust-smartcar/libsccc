#pragma once

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUTTON 2
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LED 3
#define LIBSC_USE_MOTOR 2
//#define LIBSC_USE_SERVO 2
#define LIBSC_USE_UART 1
#define LIBSC_USE_LINEAR_CCD 2
#define LIBSC_USE_LCD 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_MMA8451Q 1

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


#define LIBSC_LINEAR_CCD0_SI libbase::k60::Pin::Name::kPtb2
#define LIBSC_LINEAR_CCD0_CLK libbase::k60::Pin::Name::kPtb4
#define LIBSC_LINEAR_CCD0_AD libbase::k60::Pin::Name::kPtb7
#define LIBSC_LINEAR_CCD1_SI libbase::k60::Pin::Name::kPta28
#define LIBSC_LINEAR_CCD1_CLK libbase::k60::Pin::Name::kPtb3
#define LIBSC_LINEAR_CCD1_AD libbase::k60::Pin::Name::kPtb6

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc1Ad17

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPta27
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPta26

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta5
#define LIBSC_BUZZER_ACIVE_LEVEL 0

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc4
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtc5 // LEFT
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtc6 //DOWN
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtc3 //SELECT
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtc7 //RIGHT

#define LIBSC_LED0 libbase::k60::Pin::Name::kPtd15
#define LIBSC_LED1 libbase::k60::Pin::Name::kPtd11
#define LIBSC_LED2 libbase::k60::Pin::Name::kPtd4

#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtc9
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtc16
//#define LIBSC_MOTOR0_DEADTIME 1000
//#define LIBSC_ALTERNATE_MOTOR_CW_PWM 1
#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtc8
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPtc15
//#define LIBSC_MOTOR1_DEADTIME 1000

//#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPtc4
//#define LIBSC_SERVO1 libbase::k60::Pin::Name::kPtc3

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtb17
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtb16

#define LIBSC_MPU6050_SCL libbase::k60::Pin::Name::kPtb0
#define LIBSC_MPU6050_SDA libbase::k60::Pin::Name::kPtb1

#define LIBSC_MMA8451Q0_SCL libbase::k60::Pin::Name::kPtb0
#define LIBSC_MMA8451Q0_SDA libbase::k60::Pin::Name::kPtb1

