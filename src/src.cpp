#include <Arduino.h>
#include "define.h"

#include <ESP32Servo.h>

Servo myServo; // create servo object to control a servo

// Possible PWM GPIO pins on the ESP32-C3:
// 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

int servoPin = 2;
int potPin = 4;

int ADC_Max = 4096;

int val;

void setup()
{
    delay(2000);

    Serial.begin(115200);
    pinMode(PIN_LED_INBUILT, OUTPUT);

    digitalWrite(PIN_LED_INBUILT, HIGH);
    delay(1000);
    digitalWrite(PIN_LED_INBUILT, LOW);
    delay(1000);

    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myServo.setPeriodHertz(50);
    myServo.attach(servoPin, 500, 2400);
}

void loop()
{
    val = analogRead(potPin);
    val = map(val, 0, ADC_Max, 0, 180);
    myServo.write(val);
    delay(200);
}
