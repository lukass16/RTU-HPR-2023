#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"

class PreparationState : public State
{
public:
    byte command = 0x00;

    void start() override
    {
        Serial.println("PREPARATION STATE");

        serialcomms::setup();

        // setup motor

        // setup IMU

        // setup PID

        while (true)
        {
            command = serialcomms::readAndRespondCommand(true);
            if (command == GO_IDLE)
            {
                break;
            }

            delay(10); // some action
        }
    }
};