
#include <Arduino.h>
#include "define.h"

Servo servoSpeed;
Servo servoSteering;

int speed = 90;    // Default position for Servo 1
int steering = 90; // Default position for Servo 2

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

// Callback function when data is received via ESP-NOW
void onDataReceive(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    digitalWrite(PIN_LED_INBUILT, HIGH);
    delay(100);
    digitalWrite(PIN_LED_INBUILT, LOW);

    // Cast the data to the struct
    memcpy(&incomingData, incomingData, sizeof(incomingData));

    // Map the received values from -180 to 180 into 0 to 180 (servo range)
    int mappedServo1Angle = map(incomingData.servo1_angle, -180, 180, 0, 180);
    int mappedServo2Angle = map(incomingData.servo2_angle, -180, 180, 0, 180);

    // Write to servos
    if (angle1 == 90)
    {
        servoSpeed.detach();
    }
    else
    {
        servoSpeed.attach(SERVO_PIN_MOVE);
        servoSpeed.write(angle1);
    }

    if (angle2 == 90)
    {
        servoSteering.detach();
    }
    else
    {
        servoSteering.attach(SERVO_PIN_TURN);
        servoSteering.write(angle2);
    }

    Serial.printf("Servo Angle: %03d, %03d\n", angle1, angle2);
}

void setup()
{
    // put your setup code here, to run once:

    // Enables Serial Communication with baudRate of 115200
    Serial.begin(115200);

    pinMode(PIN_LED_INBUILT, OUTPUT);

    Serial.println("PlatformIO ESP32 Boilerplate started...");

    // Attach servos to pins
    servoSpeed.attach(SERVO_PIN_MOVE);
    servoSteering.attach(SERVO_PIN_TURN);

    // Set initial position
    servoSpeed.write(speed);
    servoSteering.write(steering);

    // Initialize WiFi in station mode
    WiFi.mode(WIFI_STA);

    // Print the MAC address of this receiver
    Serial.print("Receiver ESP32 MAC Address: ");
    Serial.println(WiFi.macAddress());

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register receive callback
    esp_now_register_recv_cb(onDataReceive);

    Serial.println("ESP-NOW Servo Control Ready.");
}

void loop()
{
    // Nothing to do in loop, everything is handled in the callback
}

#endif
