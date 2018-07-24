#pragma once

#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 8
#define LIBSC_USE_SERVO 4
#define LIBSC_USE_UART 4
#define LIBSC_USE_ENCODER 1

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta10
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta11

#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtd11
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtd12
#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtd13
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtd14
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtd15

#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPte0
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPte1
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPte2
#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPte3
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPte4

#define LIBSC_LED0 libbase::k60::Pin::Name::kPta4
#define LIBSC_LED1 libbase::k60::Pin::Name::kPta5
#define LIBSC_LED2 libbase::k60::Pin::Name::kPta6
#define LIBSC_LED3 libbase::k60::Pin::Name::kPta7


#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtc1
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtc2

#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtc3
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPtc4

#define LIBSC_MOTOR2_PWM libbase::k60::Pin::Name::kPtd4
#define LIBSC_MOTOR2_DIR libbase::k60::Pin::Name::kPtd5

#define LIBSC_MOTOR3_PWM libbase::k60::Pin::Name::kPtd6
#define LIBSC_MOTOR3_DIR libbase::k60::Pin::Name::kPtd7

#define LIBSC_MOTOR4_PWM libbase::k60::Pin::Name::kPte5
#define LIBSC_MOTOR4_DIR libbase::k60::Pin::Name::kPte6

#define LIBSC_MOTOR5_PWM libbase::k60::Pin::Name::kPte7
#define LIBSC_MOTOR5_DIR libbase::k60::Pin::Name::kPte8

#define LIBSC_MOTOR6_PWM libbase::k60::Pin::Name::kPte9
#define LIBSC_MOTOR6_DIR libbase::k60::Pin::Name::kPte10

#define LIBSC_MOTOR7_PWM libbase::k60::Pin::Name::kPte11
#define LIBSC_MOTOR7_DIR libbase::k60::Pin::Name::kPte12


#define LIBSC_MOTOR0_PWMA libbase::k60::Pin::Name::kPtc1
#define LIBSC_MOTOR0_PWMB libbase::k60::Pin::Name::kPtc2
#define LIBSC_MOTOR0_DEADTIME 1000

#define LIBSC_MOTOR1_PWMA libbase::k60::Pin::Name::kPtc3
#define LIBSC_MOTOR1_PWMB libbase::k60::Pin::Name::kPtc4
#define LIBSC_MOTOR1_DEADTIME 1000

#define LIBSC_MOTOR2_PWMA libbase::k60::Pin::Name::kPtd4
#define LIBSC_MOTOR2_PWMB libbase::k60::Pin::Name::kPtd5
#define LIBSC_MOTOR2_DEADTIME 1000

#define LIBSC_MOTOR3_PWMA libbase::k60::Pin::Name::kPtd6
#define LIBSC_MOTOR3_PWMB libbase::k60::Pin::Name::kPtd7
#define LIBSC_MOTOR3_DEADTIME 1000

#define LIBSC_MOTOR4_PWMA libbase::k60::Pin::Name::kPte5
#define LIBSC_MOTOR4_PWMB libbase::k60::Pin::Name::kPte6
#define LIBSC_MOTOR4_DEADTIME 1000

#define LIBSC_MOTOR5_PWMA libbase::k60::Pin::Name::kPte7
#define LIBSC_MOTOR5_PWMB libbase::k60::Pin::Name::kPte8
#define LIBSC_MOTOR5_DEADTIME 1000

#define LIBSC_MOTOR6_PWMA libbase::k60::Pin::Name::kPte9
#define LIBSC_MOTOR6_PWMB libbase::k60::Pin::Name::kPte10
#define LIBSC_MOTOR6_DEADTIME 1000

#define LIBSC_MOTOR7_PWMA libbase::k60::Pin::Name::kPte11
#define LIBSC_MOTOR7_PWMB libbase::k60::Pin::Name::kPte12
#define LIBSC_MOTOR7_DEADTIME 1000

#define LIBSC_ALTERNATE_MOTOR_CW_PWM 1

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPta8
#define LIBSC_SERVO1 libbase::k60::Pin::Name::kPta9
#define LIBSC_SERVO2 libbase::k60::Pin::Name::kPta10
#define LIBSC_SERVO3 libbase::k60::Pin::Name::kPta11

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPte25
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPte24
#define LIBSC_UART1_TX libbase::k60::Pin::Name::kPtb11
#define LIBSC_UART1_RX libbase::k60::Pin::Name::kPtb10
#define LIBSC_UART2_TX libbase::k60::Pin::Name::kPtb17
#define LIBSC_UART2_RX libbase::k60::Pin::Name::kPtb16
#define LIBSC_UART3_TX libbase::k60::Pin::Name::kPtd3
#define LIBSC_UART3_RX libbase::k60::Pin::Name::kPtd2
