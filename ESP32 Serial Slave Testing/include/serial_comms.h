#pragma once

#include "Arduino.h"

// defining significant command bytes
#define GO_IDLE 0x01
#define START_STABILIZATION 0x02
#define START_POINTING 0x03
#define INCREMENT 0x04
#define DECREMENT 0x05
#define MOTOR_DATA 0x06
#define RESPONSE_BYTE 0XAA

// defining buffer sizes
#define CHECKSUM_SIZE 2
#define BUFFER_SIZE 256

// defining sender id
#define LOCAL_ID 0x54
#define SENDER_ID 0x27


namespace serialcomms
{
    byte received[BUFFER_SIZE];   // received data
    byte data[BUFFER_SIZE];       // data to be sent out
    byte checksum[CHECKSUM_SIZE]; // local checksum variable for checksum calculations

    void setup()
    {
        Serial2.begin(115200);
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
            sum += (data[i]) ^ 0x55;
        }
        checksum[1] = sum;
        return checksum;
    }

    bool compareChecksum(byte *receivedChecksum, byte *calculatedChecksum)
    {
        for (int i = 0; i < CHECKSUM_SIZE; i++)
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
        for (int i = 0; i < CHECKSUM_SIZE; i++)
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

    void printPacketDetails(byte *data, int len, int id, byte *receivedCheckSum, bool hex = false)
    {
        Serial.print("\nLength: " + String(len));
        Serial.print("\nID: " + String(id));
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

    /* General sending-receiving functionality */
    void sendPacket(byte *data, int len)
    {
        Serial2.write(LOCAL_ID);
        Serial2.write(len);
        Serial2.write(data, len);
        Serial2.write(calculateChecksum(data, len), CHECKSUM_SIZE);
        Serial2.flush();
    }

    void sendByte(byte b)
    {
        data[0] = b;

        Serial2.write(LOCAL_ID);
        Serial2.write(1);
        Serial2.write(b);
        Serial2.write(calculateChecksum(data, 1), CHECKSUM_SIZE);
        Serial2.flush();
    }

    void sendSlaveResponse()
    {
        data[0] = RESPONSE_BYTE;

        Serial2.write(LOCAL_ID);
        Serial2.write(1);
        Serial2.write(RESPONSE_BYTE);
        Serial2.write(calculateChecksum(data, 1), CHECKSUM_SIZE);
        Serial2.flush();
    }

    int readPacket(bool verbose = false)
    {
        while (Serial2.available())
        {
            uint8_t id = Serial2.read();
            if(id != SENDER_ID) // if this is not the start of a message from the expected sender - skip
            {
                continue;
            }

            uint8_t len = Serial2.read();
            for (int i = 0; i < len; i++)
            {
                received[i] = Serial2.read();
            }

            byte receivedChecksum[CHECKSUM_SIZE];
            for (int i = 0; i < CHECKSUM_SIZE; i++)
            {
                receivedChecksum[i] = Serial2.read();
            }
            byte *calculatedChecksum = calculateChecksum(received, len);

            if (compareChecksum(receivedChecksum, calculatedChecksum) && len > 0 && id != LOCAL_ID)
            {
                if (verbose)
                {
                    printPacketDetails(received, len, id, receivedChecksum, true);
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

    void clearCommand()
    {
        received[0] = 0x00;
    }

    /* Specific sending-receiving functionality */
    bool sendCommand(byte command, bool verbose = false)
    {
        static bool success = 0; // local variable that remembers if command sent successfully
        serialcomms::sendByte(command);
        delay(10); // slight delay for receiving response byte
        int len = serialcomms::readPacket(true);
        if (len == 1)
        {
            byte response = serialcomms::readCommand();
            clearCommand(); // clean buffer for next command
            if (response == RESPONSE_BYTE)
            {
                success = 1;
            }
            else
            {
                success = 0;
            }
        }
        else
        {
            success = 0;
        }
        if(!success && verbose)
        {
            Serial.println("Failed to retrieve response byte");
        }
        return success;
    }

    byte readAndRespondCommand(bool verbose = false)
    {
        byte command = 0x00;
        int len = serialcomms::readPacket(verbose);
        delayMicroseconds(500); // slight delay between receiving and responding
        if (len == 1)
        {
            command = readCommand();
            serialcomms::sendSlaveResponse();
        }
        return command;
    }

    //* Development
    uint8_t getHexCommandFromSerial(bool verbose = false)
    {
        static String readString;
        static char readBuffer[8]; // buffer of size 8
        int command = 0;
        while (Serial.available())
        {
            readString += Serial.readString(); // read string from serial
        }
        if (readString != "")
        {
            delay(10);
            readString.toCharArray(readBuffer, 8);
            command = strtoul(readBuffer, NULL, 16);
        }
        readString = "";
        if (verbose && command != 0)
        {
            Serial.println("Read command:\tDEC: " + String(command) + "\tHEX: " + String(command, HEX));
        }

        return command;
    }

    int testRead()
    {
        if (Serial2.available())
        {
            uint8_t testByte = Serial2.read();
            Serial.println("Received: " + String(testByte, HEX));

            return testByte;
        }
        return -1;
    }

}
