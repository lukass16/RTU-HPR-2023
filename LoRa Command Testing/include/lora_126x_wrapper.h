#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>

// https://github.com/JJJS777/Hello_World_RadioLib_Heltec-V3_SX1262/blob/main/examples/LoRa_Transmitter/radioLib_SX1262_tx_example.cpp

#define MOSI 10
#define MISO 11
#define SCK 9

#define SS 8
#define DIO1 14
#define RST 12
#define BUSY 13

SX1262 radio = new Module(SS, DIO1, RST, BUSY);

namespace lora
{
    void setup(double freq = 434.1)
    {
        // SPI LoRa pins
        SPI.begin(SCK, MISO, MOSI, SS);

        // initialize SX1262 with default settings
        Serial.print(F("Initializing SX1262 LoRa"));
        int state = radio.begin(freq, 125, 9, 7, 0x34, 10);
        if (state == RADIOLIB_ERR_NONE)
        {
            Serial.println("Lora initialized with frequency: " + String(freq) + " MHz");
        }
        else
        {
            Serial.print(F("Initalization failed, status code "));
            Serial.println(state);
            while (true)
                ;
        }
    }

    int sendMessage(String outgoing, bool verbose = false)
    {
        int state = radio.transmit(outgoing);

        //*Development
        /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF}; can also transmit byte array up to 256 bytes long
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
};