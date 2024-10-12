#include <Arduino.h>
#include "define.h"

#include <ESP32Servo.h>

#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEUtils.h>

// BLE settings
#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHAR_UUID1 "12345678-1234-5678-1234-56789abcdef1"
#define CHAR_UUID2 "12345678-1234-5678-1234-56789abcdef2"

BLEClient *pClient;
BLERemoteCharacteristic *pRemoteCharacteristic1;
BLERemoteCharacteristic *pRemoteCharacteristic2;

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

    Serial.begin(115200);
    BLEDevice::init("ESP32-C3 Receiver");

    // Connect to the transmitter
    pClient = BLEDevice::createClient();
    pClient->connect(BLEAddress("00:00:00:00:00:00")); // Replace with transmitter's MAC address
    Serial.println("Connected to transmitter.");

    BLERemoteService *pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr)
    {
        Serial.println("Failed to find service.");
        pClient->disconnect();
        return;
    }

    pRemoteCharacteristic1 = pRemoteService->getCharacteristic(CHAR_UUID1);
    pRemoteCharacteristic2 = pRemoteService->getCharacteristic(CHAR_UUID2);

    if (pRemoteCharacteristic1)
    {
        pRemoteCharacteristic1->registerForNotify([](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify)
                                                  { Serial.printf("Received on Channel 1: %d\n", data[0]); });
    }
    else
    {
        Serial.println("Failed to find characteristic 1.");
    }

    if (pRemoteCharacteristic2)
    {
        pRemoteCharacteristic2->registerForNotify([](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify)
                                                  { Serial.printf("Received on Channel 2: %d\n", data[0]); });
    }
    else
    {
        Serial.println("Failed to find characteristic 2.");
    }
}

void loop()
{
    val = analogRead(potPin);
    val = map(val, 0, ADC_Max, 0, 180);
    myServo.write(val);
    delay(200);
}
