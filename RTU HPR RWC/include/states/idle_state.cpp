#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "serial_comms.h"
#include "buzzer.h"
#include "motor_wrapper.h"

class IdleState : public State
{
public:
    byte command = 0x00;

    void start() override
    {
        Serial.println("IDLE STATE");
        buzzer::signalIdleMode();

        //* set wheel speed to 0
        motor::setWheelSpeed(0);

        // //* Testing
        // int interval = 10000; // loop interval in ms
        // unsigned long start_t = millis();
        while (true)
        {
            // read and act upon command
            command = serialcomms::readAndRespondCommand(true);
            if (command == START_STABILIZATION)
            {
                break;
            }

            delay(10); // some action
        }
    }
};