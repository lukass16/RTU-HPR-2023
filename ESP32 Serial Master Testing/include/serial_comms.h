#pragma once

#include "Arduino.h"

namespace serialcomms
{
    byte received[256];
    byte data[256];

    void setup()
    {
        Serial2.begin(115200); // hardware serial 2 (RX-16, TX-17)
        Serial.println("Starting serial comms");
    }

    void encodeFloat(float f, byte *buf, int offset)
    {
        byte *b = (byte *)&f;
        for (int i = 0; i < 4; i++)
        {
            buf[offset + i] = b[i];
        }
    }

    void decodeFloat(float *f, byte *buf, int offset)
    {
        byte *b = (byte *)f;
        for (int i = 0; i < 4; i++)
        {
            b[i] = buf[offset + i];
        }
    }

    //* Testing
    void prepareDataPacket(float num)
    {
        encodeFloat(num, data, 0);
    }

    byte calculateCheckSum(byte *data, int len)
    {
        byte sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += data[i];
        }
        return sum;
    }

    void sendPacket(byte *data, int len)
    {
        Serial2.write(len);
        Serial2.write(data, len);
        Serial2.write(calculateCheckSum(data, len));
    }

    //* Testing
    void displayPacket(byte *data, int len, bool hex = false)
    {
        Serial.print("Message: ");
        for (int i = 0; i < len; i++)
        {
            Serial.print(data[i], hex ? HEX : DEC);
            Serial.print(" ");
        }
        Serial.println("Length: " + String(len));
    }

    int readPacket()
    {
        if (Serial2.available())
        {
            uint8_t len = Serial2.read();
            int count = 0;

            for (int i = 0; i < len; i++)
            {
                received[i] = Serial2.read();
                count += 1;
            }
            byte receivedCheckSum = Serial2.read();

            if (receivedCheckSum == calculateCheckSum(received, len) && len > 1) // len <= 1 added to safeguard against empty messages
            {
                return len;
            }
            else
            {
                return -1;
            }
        }
        return -1;
    }
}
