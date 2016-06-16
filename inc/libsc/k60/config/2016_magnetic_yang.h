#pragma once

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUTTON 2
#define LIBSC_USE_PASSIVE_BUZZER 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_UART 1
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_MMA8451Q 1
#define LIBSC_USE_MAGSEN 6

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc0Ad14

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPta4
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPte28

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta5
#define LIBSC_PASSSIVE_BUZZER0_PIT_CHANNEL 0

#define LIBSC_MPU6050_SCL libbase::k60::Pin::Name::kPtb2
#define LIBSC_MPU6050_SDA libbase::k60::Pin::Name::kPtb3


#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPtb0
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPtb1

#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPtb18
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPtb19

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPta12
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPta13
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPta14
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPta15
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPta16

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPte3
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPte0
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPte4
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPte1
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPte2

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte27
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte26
#define LIBSC_LED2 libbase::k60::Pin::Name::kPte25
#define LIBSC_LED3 libbase::k60::Pin::Name::kPte24

#define LIBSC_MOTOR0_PWMA libbase::k60::Pin::Name::kPtc8
#define LIBSC_MOTOR0_PWMB libbase::k60::Pin::Name::kPtc9
#define LIBSC_MOTOR0_DEADTIME 1000

#define LIBSC_MOTOR1_PWMA libbase::k60::Pin::Name::kPtc10
#define LIBSC_MOTOR1_PWMB libbase::k60::Pin::Name::kPtc11
#define LIBSC_MOTOR1_DEADTIME 1000
#define LIBSC_ALTERNATE_MOTOR_CW_PWM 1

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPte8
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPte9

#define LIBSC_MMA8451Q0_SCL libbase::k60::Pin::Name::kPtd8
#define LIBSC_MMA8451Q0_SDA libbase::k60::Pin::Name::kPtd9

#define LIBSC_MAGSEN0 libbase::k60::Pin::Name::kPta11
#define LIBSC_MAGSEN1 libbase::k60::Pin::Name::kPta10
#define LIBSC_MAGSEN2 libbase::k60::Pin::Name::kPta9
#define LIBSC_MAGSEN3 libbase::k60::Pin::Name::kPta6
#define LIBSC_MAGSEN4 libbase::k60::Pin::Name::kPta7
#define LIBSC_MAGSEN5 libbase::k60::Pin::Name::kPta8
