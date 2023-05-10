#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"
#include "motor_wrapper.h"
#include "pid_wrapper.h"

class StabilizationState : public State
{
public:
    byte command = 0x00;
    float wheelSpeed = 0;

    void start() override
    {
        Serial.println("STABILIZATION STATE");
        buzzer::signalStabilizationMode();

        // set stabilziation mode
        pidcontrol::setMode(STABILIZE);

        //* Testing
        int interval = 10000; // loop interval in ms
        unsigned long start_t = millis();
        while (millis() - start_t < interval)
        {
            // read and act upon command
            command = serialcomms::readAndRespondCommand(true);
            if (command == START_POINTING)
            {
                break;
            }

            // PID control
            imu::readSensor();
            wheelSpeed = pidcontrol::update(0);

            //* Testing
            pidcontrol::printMode();
            Serial.println("Wheel Speed: " + String(wheelSpeed, 2));
            Serial.println("Plat Angular Velocity: " + String(imu::getGyrZ(), 2));
            Serial.println("Plat Orientation: " + String(imu::getPlatOrientation(), 2));
            pidcontrol::printTerms();

            motor::setWheelSpeed(wheelSpeed);

            delay(50); // some action
        }
    }
};