#pragma once

#include "Arduino.h"
#include "imu_wrapper.h"

#define PWM_PIN 33
#define DIRECTION_PIN 32
#define A_PIN 37
#define B_PIN 36 // it was determined that an ISR routine on this pin executes not only when it is rising, but also when pin 37 is falling, be aware of this

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
        // setting pins
        pinMode(DIRECTION_PIN, OUTPUT);
        pinMode(A_PIN, INPUT_PULLDOWN);
        pinMode(B_PIN, INPUT_PULLDOWN);

        ledcSetup(channel, freq, resolution); // setting up the PWM channel
        ledcAttachPin(PWM_PIN, channel);      // attaching the piezo_pin to the PWM channel
        Serial.println("Motor setup finished. PWM pin: " + String(PWM_PIN) + ", direction pin: " + String(DIRECTION_PIN));
    }

    void setupEncoder()
    {
        attachInterrupt(A_PIN, isrA, RISING);
        prevTime = millis();
    }

    void setDutyCycle(int duty_cycle) // 0 - 255
    {
        ledcWrite(channel, duty_cycle);
    }

    void setFrequency(int freq)
    {
        ledcWriteTone(channel, freq);
    }

    void destroyPWM()
    {
        ledcDetachPin(PWM_PIN);
    }

    void setDirection(int direction)
    {
        digitalWrite(DIRECTION_PIN, direction); // HIGH or LOW
    }

    int readChannelA()
    {
        return analogRead(A_PIN);
    }

    int readChannelB()
    {
        return analogRead(B_PIN);
    }

    float getRotationalFrequency()
    {
        // calculate amount of rotations
        rotations = (float)encCounter / ENC_COUNTS_PER_REV;
        encCounter = 0;

        // calculate change in time
        curTime = millis();
        dt = (curTime - prevTime) / 1000.0; // in seconds
        prevTime = curTime;

        // calculate rotational frequency
        rotFreq = (rotations / dt); // sign depends on direction of rotation
        if (rotFreqAvgCounter < rotFreqAvgCycles)
        {
            rotFreqAvgSum += rotFreq;
            rotFreqAvgCounter++;
        }
        else
        {
            rotFreqAvg = rotFreqAvgSum / rotFreqAvgCycles;
            rotFreqAvgSum = 0;
            rotFreqAvgCounter = 0;
        }

        // calculate rpm
        rpm = rotFreq * 60; // note: not calculated from average
        rpmAvg = rotFreqAvg * 60;

        return rotFreqAvg;
    }

    void setWheelSpeed(float wheelSpeed)
    {
        static float wheelSpeedPrev = 0;

        // convert to duty cycle and motor direction output
        if (wheelSpeed != wheelSpeedPrev)
        {
            if (wheelSpeed >= 0)
            {
                dutyCycle = round(wheelSpeed * 255);
                direction = 0;
            }
            else
            {
                dutyCycle = -round(wheelSpeed * 255);
                direction = 1;
            }
        }

        Serial.println(String(dutyCycle) + "\t" + String(direction));

        // sending stabilizing commands to motor
        motor::setDutyCycle(dutyCycle);
        motor::setDirection(direction);

        wheelSpeedPrev = wheelSpeed;
    }

    void testCountdown()
    {
        int duration = 5;
        Serial.print("Starting test in:");
        for(int i = 0; i < duration; i++)
        {
            Serial.print(" " + String(duration - i));
            delay(1000);
        }
        Serial.println();
    }
}
