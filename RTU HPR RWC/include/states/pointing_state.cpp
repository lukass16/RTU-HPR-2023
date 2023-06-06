#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"
#include "motor_wrapper.h"
#include "pid_wrapper.h"

class PointingState : public State
{
public:
    byte command = 0x00;
    float wheelSpeed = 0;

    void start() override
    {
        Serial.println("POINTING STATE");
        buzzer::signalPointingMode();

        // set pointing mode
        pidcontrol::setMode(POINT);

        // //* Testing
        // int interval = 10000; // loop interval in ms
        // unsigned long start_t = millis();
        while (true)
        {
            // read and act upon command
            command = serialcomms::readAndRespondCommand(true);
            if (command == GO_IDLE)
            {
                break;
            }
            else if (command == INCREMENT)
            {
                // increment PID setpoint
            }
            else if (command == DECREMENT)
            {
                // decrement PID setpoint
            }

            // PID control
            imu::readSensor();
            wheelSpeed = pidcontrol::update(160);

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