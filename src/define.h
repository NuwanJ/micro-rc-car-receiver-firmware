#pragma once

#include <Arduino.h>

#include <Servo.h>
#include <esp_now.h>

// Load the configuration details
#include "config/config.h"

// pin map
#include "config/pins.h"

// Structure to receive data
typedef struct struct_message
{
    int servo1_angle; // Servo 1 angle (-180 to 180)
    int servo2_angle; // Servo 2 angle (-180 to 180)
} struct_message;