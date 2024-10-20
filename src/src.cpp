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

Servo myServoM;
Servo myServoS;

int val;

int valX, valY;
int oldX = 90, oldY = 90;

bool isAttachedX = false;
bool isAttachedY = false;

void setup()
{
    delay(2000);

    Serial.begin(115200);
    Serial.println("ESP32-C3 Receiver");
    pinMode(PIN_LED_INBUILT, OUTPUT);

    digitalWrite(PIN_LED_INBUILT, LOW);
    delay(500);

    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    myServoM.setPeriodHertz(50);
    myServoS.setPeriodHertz(50);

    Serial.println("ESP32-C3 Receiver");
    BLEDevice::init("ESP32-C3 Receiver");

    digitalWrite(PIN_LED_INBUILT, HIGH);
    delay(500);
    digitalWrite(PIN_LED_INBUILT, LOW);
    delay(500);
    digitalWrite(PIN_LED_INBUILT, HIGH);

    // Connect to the transmitter
    pClient = BLEDevice::createClient();
    pClient->connect(BLEAddress("40:4c:ca:f9:e1:76")); // Replace with transmitter's MAC address

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
                                                  { 
                                                    // Serial.printf("Received on Channel 1: %d\n", data[0]);
                                                    valX = data[0]; });
    }

    if (pRemoteCharacteristic2)
    {
        pRemoteCharacteristic2->registerForNotify([](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify)
                                                  { 
                                                    // Serial.printf("Received on Channel 2: %d\n", data[0]);
                                                    valY = data[0]; });
    }
}

void writeX(int val)
{
    int threshold = abs(val - 90);

    if (threshold > 4 && !isAttachedX)
    {
        myServoM.attach(SERVO_PIN_MOVE, 500, 2400);
        isAttachedX = true;
    }

    if (threshold <= 4)
    {
        myServoM.detach();
        isAttachedX = false;
        oldX = 90;
    }
    else if (oldX != val)
    {
        Serial.printf("M: %d -> %d\n", oldX, val);
        myServoM.write(val);
        oldX = val;
    }
}

void writeY(int val)
{
    int threshold = abs(val - 90);

    if (threshold > 4 && !isAttachedY)
    {
        myServoS.attach(SERVO_PIN_TURN, 500, 2400);
        isAttachedY = true;
    }

    if (threshold <= 4)
    {
        myServoS.detach();
        isAttachedY = false;
        oldY = 90;
    }
    else if (oldY != val)
    {
        Serial.printf("S: %d -> %d\n", oldY, val);
        myServoS.write(val);
        oldY = val;
    }
}

void loop()
{
    writeX(45 + (90 - ((valX + 15) / 2))); // motor
    writeY(180 - (valY + 25));
    // delay(200);
}
