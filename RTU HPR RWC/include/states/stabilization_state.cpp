#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"

class StabilizationState : public State
{
public:
    byte command = 0x00;

    void start() override
    {
        Serial.println("STABILIZATION STATE");

        while (true)
        {
            command = serialcomms::readAndRespondCommand(true);
            if (command == START_POINTING)
            {
                break;
            }

            delay(10); // some action
        }
    }
};