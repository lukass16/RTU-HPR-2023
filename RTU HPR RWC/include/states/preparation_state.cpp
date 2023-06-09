#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"
#include "motor_wrapper.h"
#include "pid_wrapper.h"

class PreparationState : public State
{
public:
    byte command = 0x00;

    void start() override
    {
        Serial.println("PREPARATION STATE");
        buzzer::setup();

        serialcomms::setup();

        // setup motor
        motor::setup();
        motor::setupEncoder();

        // setup IMU
        imu::setup(); //! commented out for testing

        // setup PID
        pidcontrol::setup();

        // calibrate sensor
        imu::bruteForceCalibration(); //! commented out for testing

        //* Testing
        // warn of start
        buzzer::signalStartWarning();

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

            delay(50); // some action
        }
    }
};