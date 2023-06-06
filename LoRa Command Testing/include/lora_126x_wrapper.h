#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>

// https://github.com/JJJS777/Hello_World_RadioLib_Heltec-V3_SX1262/blob/main/examples/LoRa_Transmitter/radioLib_SX1262_tx_example.cpp

#define LORA_MOSI 10
#define LORA_MISO 11
#define LORA_SCK 9

#define LORA_SS 8
#define LORA_DIO1 14
#define LORA_RST 12
#define LORA_BUSY 13

SX1262 radio = new Module(LORA_SS, LORA_DIO1, LORA_RST, LORA_BUSY);

namespace lora
{
    void setup(float freq = 434)
    {
        // SPI LoRa pins
        SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);

        // initialize SX1262 with default settings
        Serial.println("Initializing SX1262 LoRa");
        int state = radio.begin(freq, 125, 9, 7, 0x34, 10);
        if (state == RADIOLIB_ERR_NONE)
        {
            Serial.println("Lora initialized with frequency: " + String(freq, 2) + " MHz");
        }
        else
        {
            Serial.print("Initalization failed, status code ");
            Serial.println(state);
            while (true)
                ;
        }
    }

    void send(String outgoing, bool verbose = false)
    {
        if (verbose)
        {
            Serial.print(F("[SX1262] Transmitting packet ... "));
        }

        int state = radio.transmit(outgoing);

        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF};
          int state = radio.transmit(byteArr, 8);
        */

        if (state == RADIOLIB_ERR_NONE)
        {
            if (verbose)
            {
                // the packet was successfully transmitted
                Serial.println(F("Message sent successfully"));
                // print measured data rate
                Serial.print(F("SX1262 Datarate:\t"));
                Serial.print(radio.getDataRate());
                Serial.println(F(" bps"));
            }
        }
        else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
        {
            // the supplied packet was longer than 256 bytes
            Serial.println(F("Failed to send packet: packet too long"));
        }
        else if (state == RADIOLIB_ERR_TX_TIMEOUT)
        {
            // timeout occured while transmitting packet
            Serial.println(F("Failed to send packet: timeout occured"));
        }
        else
        {
            // some other error occurred
            Serial.print(F("Failed to send packet: status code "));
            Serial.println(state);
        }
    }

    void sendCommand(byte command, bool verbose = false)
    {
        if (verbose)
        {
            Serial.print(F("[SX1262] Transmitting command ... "));
        }

        byte buffer[1] = {command};
        int state = radio.transmit(buffer, 1);

        if (state == RADIOLIB_ERR_NONE)
        {
            if (verbose)
            {
                // the packet was successfully transmitted
                Serial.println(F("Message sent successfully"));
                // print measured data rate
                Serial.print(F("SX1262 Datarate:\t"));
                Serial.print(radio.getDataRate());
                Serial.println(F(" bps"));
            }
        }
        else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
        {
            // the supplied packet was longer than 256 bytes
            Serial.println(F("Failed to send command: packet too long"));
        }
        else if (state == RADIOLIB_ERR_TX_TIMEOUT)
        {
            // timeout occured while transmitting packet
            Serial.println(F("Failed to send command: timeout occured"));
        }
        else
        {
            // some other error occurred
            Serial.print(F("Failed to send command: status code "));
            Serial.println(state);
        }
    }

    void receive(bool verbose = false)
    {
        if (verbose)
        {
            Serial.println(F("[SX1262] Waiting for incoming transmission ... "));
        }

        String str;
        int state = radio.receive(str);

        // you can also receive data as byte array
        /*
          byte byteArr[8];
          int state = radio.receive(byteArr, 8);
        */

        if (state == RADIOLIB_ERR_NONE)
        {
            Serial.print(F("Received packet:\t"));
            Serial.println(str);

            if (verbose)
            {
                Serial.print(F("[SX1262] RSSI:\t\t"));
                Serial.print(radio.getRSSI());
                Serial.println(F(" dBm"));

                Serial.print(F("[SX1262] SNR:\t\t"));
                Serial.print(radio.getSNR());
                Serial.println(F(" dB"));

                Serial.print(F("[SX1262] Frequency error:\t"));
                Serial.print(radio.getFrequencyError());
                Serial.println(F(" Hz"));
            }
        }
        else if (state == RADIOLIB_ERR_RX_TIMEOUT)
        {
            if (verbose)
            {
                // timeout occurred while waiting for a packet
                Serial.println(F("Failed to receive packet: timeout occured"));
            }
        }
        else if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            if (verbose)
            {
                // packet was received, but is malformed
                Serial.println(F("Failed to receive packet: CRC error encountered"));
            }
        }
        else
        {
            if (verbose)
            {
                // some other error occurred
                Serial.print(F("Failed to receive packet: status code "));
                Serial.println(state);
            }
        }
    }

    byte receiveCommand(bool verbose = false)
    {
        if (verbose)
        {
            Serial.println(F("[SX1262] Waiting for incoming transmission ... "));
        }

        byte command = 0x00;
        byte buffer[1] = {0x00};
        int state = radio.receive(buffer, 1);
        command = buffer[0];

        if (state == RADIOLIB_ERR_NONE)
        {
            Serial.print(F("Received command:\t"));
            Serial.println(command, HEX);

            if (verbose)
            {
                Serial.print(F("[SX1262] RSSI:\t\t"));
                Serial.print(radio.getRSSI());
                Serial.println(F(" dBm"));

                Serial.print(F("[SX1262] SNR:\t\t"));
                Serial.print(radio.getSNR());
                Serial.println(F(" dB"));

                Serial.print(F("[SX1262] Frequency error:\t"));
                Serial.print(radio.getFrequencyError());
                Serial.println(F(" Hz"));
            }

            return command;
        }
        else if (state == RADIOLIB_ERR_RX_TIMEOUT)
        {
            if (verbose)
            {
                // timeout occurred while waiting for a packet
                Serial.println(F("Failed to receive command: timeout occured"));
            }
        }
        else if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            if (verbose)
            {
                // packet was received, but is malformed
                Serial.println(F("Failed to receive command: CRC error encountered"));
            }
        }
        else
        {
            if (verbose)
            {
                // some other error occurred
                Serial.print(F("Failed to receive command: status code "));
                Serial.println(state);
            }
        }
        return 0x00;
    }
};