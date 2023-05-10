#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"

class IdleState : public State
{
public:
    byte command = 0x00;

    void start() override
    {
        Serial.println("IDLE STATE");
        buzzer::signalIdleMode();

         //* Testing
        delay(2000);
        // while (true)
        // {
        //     command = serialcomms::readAndRespondCommand(true);
        //     if (command == START_STABILIZATION)
        //     {
        //         break;
        //     }

        //     delay(10); // some action
        // }
    }
};