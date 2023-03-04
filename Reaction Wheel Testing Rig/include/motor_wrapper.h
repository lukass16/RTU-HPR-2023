#pragma once

#include "Arduino.h"

#define PWM_PIN 32
#define DIRECTION_PIN 33
#define A_PIN 36
#define B_PIN 37

#define ENC_COUNTS_PER_REV 100

// https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
// https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/

//! TESTING
int dutyCycle = 0, direction = 0;
volatile int encCounter = 0;

void IRAM_ATTR encoder()
{
    encCounter++;
}

namespace motor
{
    // declaring initial values for PWM channel attributes
    int freq = 4000, channel = 0, resolution = 8;

    // declaring rotational frequency calculation variables
    float dt;
    float rotations = 0, rotFreq = 0;
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
        attachInterrupt(B_PIN, encoder, RISING);
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
        Serial.println("Rotations:" + String(rotations));

        // calculate change in time
        curTime = millis();
        dt = (curTime - prevTime) / 1000.0; // in seconds
        prevTime = curTime;
        Serial.println("dt:" + String(dt));

        // calculate rotational frequency
        rotFreq = rotations / dt;

        return rotFreq;
    }

    void stabilize(float wheelSpeed)
    {
        static float wheelSpeedPrev = wheelSpeed;

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

        Serial.println(String(dutyCycle) + "\t" +  String(direction));
        
        // sending stabilizing commands to motor
        motor::setDutyCycle(dutyCycle);
        motor::setDirection(direction);

        wheelSpeedPrev = wheelSpeed;
    }
}



