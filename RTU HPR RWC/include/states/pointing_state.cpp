#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"

class PointingState : public State
{
public:
    byte command = 0x00;

    void start() override
    {
        Serial.println("POINTING STATE");
        buzzer::signalPointingMode();

         //* Testing
        delay(2000);
        // while (true)
        // {
        //     command = serialcomms::readAndRespondCommand(true);
        //     if (command == GO_IDLE)
        //     {
        //         break;
        //     }
        //     else if(command == INCREMENT)
        //     {
        //         // increment PID setpoint
        //     }
        //     else if(command == DECREMENT)
        //     {
        //         // decrement PID setpoint
        //     }

        //     delay(10); // some action
        // }
    }
};