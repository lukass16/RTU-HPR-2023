#pragma once

#include "Arduino.h"

#define RESPONSE_BYTE 0XAA

namespace serialcomms
{
    byte received[256]; // received data
    byte data[256];     // data to be sent out
    byte checksum[3];   // local checksum variable for checksum calculations

    void setup()
    {
        Serial2.begin(115200); // hardware serial 2 (RX-16, TX-17)
        Serial.println("Starting serial comms");
    }

    /* Encoding-decoding functionality */
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

    /* Checksum operations */
    byte *calculateChecksum(byte *data, int len)
    {
        byte sum = 0;
        // first checksum
        for (int i = 0; i < len; i++)
        {
            sum += ~(data[i]);
        }
        checksum[0] = sum;
        // second checksum
        sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += (data[i] >> 4);
        }
        checksum[1] = sum;
        // third checksum
        sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += (data[i]) ^ 0x55;
        }
        checksum[2] = sum;
        return checksum;
    }

    bool compareChecksum(byte *receivedChecksum, byte *calculatedChecksum)
    {
        for (int i = 0; i < 3; i++)
        {
            if (receivedChecksum[i] != calculatedChecksum[i])
            {
                return 0;
            }
        }
        return 1;
    }

    void printChecksum(byte *checksum)
    {
        for (int i = 0; i < 3; i++)
        {
            Serial.print(checksum[i], HEX);
            Serial.print(" ");
        }
    }

    /* Testing-debugging */
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

    void printPacketDetails(byte *data, int len, byte *receivedCheckSum, bool hex = false)
    {
        Serial.print("\nLength: " + String(len));
        Serial.print("\tMessage: ");
        for (int i = 0; i < len; i++)
        {
            Serial.print(data[i], hex ? HEX : DEC);
            Serial.print(" ");
        }
        Serial.print("\tReceived checksum: ");
        printChecksum(receivedCheckSum);
        Serial.print("\tCalculated checksum: ");
        printChecksum(calculateChecksum(data, len));
        Serial.println();
    }

    /* Sending-receiving functionality */
    void sendPacket(byte *data, int len)
    {
        Serial2.write(len);
        Serial2.write(data, len);
        Serial2.write(calculateChecksum(data, len), 3);

        //* Testing
        Serial.print("Sent: ");
        Serial.print(len);
        for(int i = 0; i < len; i++)
        {
            Serial.print(" ");
            Serial.print(data[i], HEX);
        }
        for(int i = 0; i < 3; i++)
        {
            Serial.print(" ");
            Serial.print(calculateChecksum(data, len)[i], HEX);
        }
        Serial.println();
    }

    void sendCommand(byte command)
    {
        data[0] = command;

        Serial2.write(1);
        Serial2.write(command);
        Serial2.write(calculateChecksum(data, 1), 3);
    }

    void sendSlaveResponse()
    {
        data[0] = RESPONSE_BYTE;

        Serial2.write(1);
        Serial2.write(RESPONSE_BYTE);
        Serial2.write(calculateChecksum(data, 1), 3);
    }

    int readPacket(bool verbose = false)
    {
        if (Serial2.available())
        {
            uint8_t len = Serial2.read();
            for (int i = 0; i < len; i++)
            {
                received[i] = Serial2.read();
            }

            byte receivedChecksum[3];
            for (int i = 0; i < 3; i++)
            {
                receivedChecksum[i] = Serial2.read();
            }
            byte *calculatedChecksum = calculateChecksum(received, len);

            if (compareChecksum(receivedChecksum, calculatedChecksum) && len > 0)
            {
                if (verbose)
                {
                    printPacketDetails(received, len, receivedChecksum, true);
                }
                return len;
            }
            else
            {
                return -1;
            }
        }
        return -1;
    }

    void printReadPacket()
    {
        if (Serial2.available())
        {
            Serial.print("Received: ");
        }
        while (Serial2.available())
        {
            Serial.print(Serial2.read(), HEX);
            Serial.print(" ");
        }
        if (Serial2.available())
        {
            Serial.println();
        }
    }

    byte readCommand()
    {
        return received[0];
    }

}
