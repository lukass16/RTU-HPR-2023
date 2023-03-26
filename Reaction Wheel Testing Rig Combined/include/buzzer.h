#pragma once

#include "Arduino.h"

#define RESONATING_FREQ 2500
#define BOP 200
#define BEEP 1000

namespace buzzer
{
    // declaring a variable for the piezo pin
    int piezo_pin = 23;

    // variable for the structure of the third switch beeping
    bool buzzing = false;
    unsigned long previousTime = 0, currentTime = 0;
    int times_buzzed = 0;

    // declaring variables for PWM channel attributes
    int freq = 400, channel = 0, resolution = 8;

    void setup()
    {
        pinMode(piezo_pin, OUTPUT);
        Serial.println("Buzzer ready!");

        // indicate start
        digitalWrite(piezo_pin, HIGH);
        delay(200);
        digitalWrite(piezo_pin, LOW);
    }

    void signalCalibrationEnd()
    {
        digitalWrite(piezo_pin, HIGH); // 1)
        delay(50);
        digitalWrite(piezo_pin, LOW);
        delay(50);
        digitalWrite(piezo_pin, HIGH); // 2)
        delay(50);
        digitalWrite(piezo_pin, LOW);
        delay(50);
        digitalWrite(piezo_pin, HIGH); // 3)
        delay(50);
        digitalWrite(piezo_pin, LOW);
    }

    void signalStartWarning()
    {
        digitalWrite(piezo_pin, HIGH); // 1)
        delay(100);
        digitalWrite(piezo_pin, LOW);
        delay(1000);
        digitalWrite(piezo_pin, HIGH); // 2)
        delay(100);
        digitalWrite(piezo_pin, LOW);
        delay(1000);
        digitalWrite(piezo_pin, HIGH); // 3)
        delay(100);
        digitalWrite(piezo_pin, LOW);
        delay(1000);
        digitalWrite(piezo_pin, HIGH); // 4)
        delay(100);
        digitalWrite(piezo_pin, LOW);
        delay(1000);
        digitalWrite(piezo_pin, HIGH); // 5)
        delay(100);
        digitalWrite(piezo_pin, LOW);
    }

    void signalStabilizeMode()
    {
        digitalWrite(piezo_pin, HIGH); // 1)
        delay(50);
        digitalWrite(piezo_pin, LOW);
    }

    void signalPointMode()
    {
        digitalWrite(piezo_pin, HIGH); // 1)
        delay(50);
        digitalWrite(piezo_pin, LOW);
        delay(100);
        digitalWrite(piezo_pin, HIGH); // 1)
        delay(50);
        digitalWrite(piezo_pin, LOW);
    }
}