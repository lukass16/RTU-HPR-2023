#include <Arduino.h>
#include <RadioLib.h>

#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_DIO1 35
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27

SX1278 radio = new Module(LORA_SS, LORA_DIO0, LORA_RST, LORA_DIO1);

namespace lora
{
    void setup(float freq = 434)
    {
        // SPI LoRa pins
        SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);

        // initialize SX1278 with default settings
        Serial.println("Initializing SX1276 LoRa");
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
            Serial.print(F("[SX1276] Transmitting packet ... "));
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
                Serial.print(F("SX1276 Datarate:\t"));
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

    void receive(bool verbose = false)
    {
        if (verbose)
        {
            Serial.println(F("[SX1276] Waiting for incoming transmission ... "));
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
                Serial.print(F("[SX1276] RSSI:\t\t"));
                Serial.print(radio.getRSSI());
                Serial.println(F(" dBm"));

                Serial.print(F("[SX1276] SNR:\t\t"));
                Serial.print(radio.getSNR());
                Serial.println(F(" dB"));

                Serial.print(F("[SX1276] Frequency error:\t"));
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
};