#pragma once

#define ESP32
// #define ESP32C3

#ifdef ESP32

#define PIN_LED_INBUILT 2
#define SERVO_PIN_MOVE 25
#define SERVO_PIN_TURN 26

#else
// Possible PWM GPIO pins on the ESP32-C3:
// 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21
#define PIN_LED_INBUILT 8
#define SERVO_PIN_MOVE 2
#define SERVO_PIN_TURN 3

#endif

// Analog Inputs --------
#define PIN_A0 2
#define PIN_A1 3
#define PIN_A2 4
#define PIN_A3 5

// Analog Outputs --------
#define PIN_ADC_0 0
#define PIN_ADC_1 1

// I2C pins ---------------
#define PIN_SCL 7
#define PIN_SDA 6

// UART pins --------------
#define PIN_TX_0 21
#define PIN_RX_0 20

// SPI pins ---------------
#define PIN_SCK 8
#define PIN_MOSI 10
#define PIN_MISO 9
