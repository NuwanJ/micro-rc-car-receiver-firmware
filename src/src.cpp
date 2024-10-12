#include <Arduino.h>
#include "define.h"
#include <ESP32Servo.h>

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Servo myServo; // create servo object to control a servo

// // Possible PWM GPIO pins on the ESP32-C3:
// // 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

// int servoPin = 2;
// int potPin = 4;

// int ADC_Max = 4096;

// int val;

// BLE service and characteristic UUIDs
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Create a BLE Characteristic
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// Callback class for server events
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Device connected");
    }

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Device disconnected");
    }
};

// Callback class for characteristic events
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string receivedData = pCharacteristic->getValue();
        if (receivedData.length() > 0)
        {
            Serial.print("Received: ");
            Serial.println(receivedData.c_str());

            // Echo the received data back to the client
            pCharacteristic->setValue(receivedData);
            pCharacteristic->notify();
        }
    }
};

void setup()
{
    delay(2000);

    //     Serial.begin(115200);
    //     pinMode(PIN_LED_INBUILT, OUTPUT);

    //     digitalWrite(PIN_LED_INBUILT, HIGH);
    //     delay(1000);
    //     digitalWrite(PIN_LED_INBUILT, LOW);
    //     delay(1000);

    //     // Allow allocation of all timers
    //     ESP32PWM::allocateTimer(0);
    //     ESP32PWM::allocateTimer(1);
    //     ESP32PWM::allocateTimer(2);
    //     ESP32PWM::allocateTimer(3);
    //     myServo.setPeriodHertz(50);
    //     myServo.attach(servoPin, 500, 2400);

    // Initialize BLE
    BLEDevice::init("ESP32-C3 BLE Echo");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create a BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("Waiting for a client to connect...");
}

void loop()
{
    // val = analogRead(potPin);
    // val = map(val, 0, ADC_Max, 0, 180);
    // myServo.write(val);
    delay(200);
}
