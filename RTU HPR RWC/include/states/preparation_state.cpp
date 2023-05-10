#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"
#include "motor_wrapper.h"

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
        imu::setup();
        
        // setup PID


        //* Testing
        delay(2000);
        // while (true)
        // {
        //     command = serialcomms::readAndRespondCommand(true);
        //     if (command == GO_IDLE)
        //     {
        //         break;
        //     }

        //     delay(10); // some action
        // }
    }
};