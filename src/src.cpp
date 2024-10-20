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
int oldX = 0, oldY = 0;

bool isAttachedX = false;
bool isAttachedY = false;

unsigned long updateTime;

void writeX(int val)
{
    // int threshold = abs(val - 90);
    val = constrain(val, 0, 180);

    if (oldX != val)
    {
        if (!isAttachedX && val != 90)
        {
            myServoM.attach(SERVO_PIN_MOVE, 500, 2400);
            isAttachedX = true;
        }

        Serial.printf("M: %d -> %d\n", oldX, val);

        if (val == 90)
        {
            myServoM.detach();
            isAttachedX = false;
        }
        else
        {
            myServoM.write(val);
        }

        updateTime = millis();
        oldX = val;
    }
}

void writeY(int val)
{
    int threshold = abs(val - 90);

    val = constrain(val, 0, 180);

    if (oldY != val)
    {
        Serial.printf("S: %d -> %d\n", oldY, val);
        myServoS.write(val);
        oldY = val;
    }
}

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
    Serial.println("Connected");

    pRemoteCharacteristic1 = pRemoteService->getCharacteristic(CHAR_UUID1);
    pRemoteCharacteristic2 = pRemoteService->getCharacteristic(CHAR_UUID2);

    if (pRemoteCharacteristic1)
    {
        pRemoteCharacteristic1->registerForNotify([](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify)
                                                  { 
                                                    // Serial.printf("Ch01: %d\n", data[0]);
                                                    valX = data[0]; });
    }

    if (pRemoteCharacteristic2)
    {
        pRemoteCharacteristic2->registerForNotify([](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify)
                                                  { 
                                                    // Serial.printf("Ch02: %d\n", data[0]);
                                                    valY = data[0]; });
    }

    myServoS.attach(SERVO_PIN_TURN, 500, 2400);

    writeX(90);
    writeY(90);
}

void loop()
{
    writeX(90 + (90 - valX) / 2); // motor
    writeY(180 - (valY - 6));

    // if (millis() - updateTime > 250)
    // {
    //     Serial.println("");
    // }
    delay(50);
}
