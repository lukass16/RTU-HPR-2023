#pragma once

#include "Arduino.h"

#define PWM_PIN 33
#define DIRECTION_PIN 32
#define A_PIN 8
#define B_PIN 18 // it was determined that an ISR routine on this pin executes not only when it is rising, but also when pin 37 is falling, be aware of this

#define ENC_COUNTS_PER_REV 100

// https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
// https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/

int dutyCycle = 0, direction = 0;
volatile int encCounter = 0, encDirection = 0;

// ISR routine for only rotational frequency
void IRAM_ATTR isrA()
{
    encCounter++;
}

// ISR routine for rotational frequency and direction
void IRAM_ATTR isrAWithDirection()
{
    encCounter++;

    if (digitalRead(B_PIN) == HIGH)
    {
        encDirection = -1;
    }
    else
    {
        encDirection = 1;
    }
}

namespace motor
{
    // declaring initial values for PWM channel attributes
    int freq = 4000, channel = 0, resolution = 8;

    // declaring rotational frequency calculation variables
    float dt;
    float rotations = 0, rotFreq = 0, rotFreqAvgSum = 0, rotFreqAvg = 0, rotFreqAvgCycles = 10, rotFreqAvgCounter = 0, rpm = 0, rpmAvg = 0;
    unsigned long prevTime, curTime;

    void setup()
    {
        pinMode(A_PIN, INPUT_PULLDOWN);
        pinMode(B_PIN, INPUT_PULLDOWN);

        //*Testing
        pinMode(6, OUTPUT);
        digitalWrite(6, HIGH);
    }

    void setupEncoder()
    {
        attachInterrupt(A_PIN, isrA, RISING);
        prevTime = millis();
    }

    int readChannelA()
    {
        return analogRead(A_PIN);
    }

    int readChannelB()
    {
        return analogRead(B_PIN);
    }

    void printEncoderCounter()
    {
        Serial.println("Encoder Counter: " + String(encCounter));
    }

    void printChannelValues()
    {
        Serial.println("A: " + String(motor::readChannelA()) + "\tB: " + String(motor::readChannelB()));
    }

}
