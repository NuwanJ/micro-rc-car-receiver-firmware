
#include <Arduino.h>
#include "define.h"

BluetoothSerial SerialBT;
Servo servoSpeed;
Servo servoSteering;

int speed = 90;    // Default position for Servo 1
int steering = 90; // Default position for Servo 2

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

void setup()
{
    // put your setup code here, to run once:

    // Enables Serial Communication with baudRate of 115200
    Serial.begin(115200);

    pinMode(PIN_LED_INBUILT, OUTPUT);

    Serial.println("PlatformIO ESP32 Boilerplate started...");
    SerialBT.begin("ESP32C3_ServoControl"); // Bluetooth name

    // Attach servos to pins
    servoSpeed.attach(SERVO_PIN_MOVE);
    servoSteering.attach(SERVO_PIN_TURN);

    // Set initial position
    servoSpeed.write(speed);
    servoSteering.write(steering);

    Serial.println("Bluetooth Servo Control Ready. Waiting for commands...");
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Check if data is received over Bluetooth
    if (SerialBT.available())
    {
        digitalWrite(PIN_LED_INBUILT, HIGH);
        delay(1000);
        digitalWrite(PIN_LED_INBUILT, LOW);
        delay(1000);

        String data = SerialBT.readStringUntil('\n'); // Read data until newline

        // Parse the received data to control both servos
        if (data.startsWith("S1:"))
        {
            speed = data.substring(3).toInt(); // Extract angle for Servo 1
            speed = constrain(speed, 0, 180);  // Ensure it's within 0-180 degrees
            servoSpeed.write(speed);           // Move Servo 1 to the specified angle
            Serial.println("Servo 1 Angle: " + String(speed));
        }

        if (data.startsWith("S2:"))
        {
            steering = data.substring(3).toInt();   // Extract angle for Servo 2
            steering = constrain(steering, 0, 180); // Ensure it's within 0-180 degrees
            servoSteering.write(steering);          // Move Servo 2 to the specified angle
            Serial.println("Servo 2 Angle: " + String(steering));
        }
    }
}

#endif
