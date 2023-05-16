#pragma once
#include <LoRa.h>
#include <SPI.h>

// https://randomnerdtutorials.com/ttgo-lora32-sx1276-arduino-ide/
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/examples/LoRaDuplex/LoRaDuplex.ino
// https://www.thethingsnetwork.org/docs/devices/bytes/

// definitions for Heltec V3, source: https://github.com/JJJS777/Hello_World_RadioLib_Heltec-V3_SX1262 
#define SS 8
#define RST 12
#define DIO1 14
#define SCK 9
#define MISO 11
#define MOSI 10

#define B_SIZE 37

namespace lora
{

    String outgoing; // outgoing message

    int syncWord = 0xF3;
    byte _localAddress = 0xFF; // address of this device
    byte _destination = 0xFF;  // destination to send to

    boolean _canPrintHeaderMessage = false;

    void sendMessage(String outgoing, int lora_message_id);

    void setup(double frequency = 434.1E6, boolean canPrintHeaderMessage = false)
    {
        _canPrintHeaderMessage = canPrintHeaderMessage;

        // SPI LoRa pins
        SPI.begin(SCK, MISO, MOSI, SS);

        // setup LoRa transceiver module
        LoRa.setPins(SS, RST, DI0);

        if (!LoRa.begin(frequency))
        {
            Serial.println("Starting LoRa failed!");
            while (1)
                ;
        }

        // setting paramaters
        LoRa.setSyncWord(0xF3);
        LoRa.setTxPower(14);
        LoRa.setSpreadingFactor(10);
        LoRa.setCodingRate4(6);
        LoRa.setSignalBandwidth(62.5E3);

        Serial.println("Lora initialized with frequency: " + String(frequency / 1e6, 1) + " MHz");
    }

    void sendMessage(String outgoing, int lora_message_id)
    {
        LoRa.beginPacket();            // start packet
        LoRa.write(_destination);      // add destination address
        LoRa.write(_localAddress);     // add sender address
        LoRa.write(lora_message_id);   // add message ID
        LoRa.write(outgoing.length()); // add payload length
        LoRa.print(outgoing);          // add payload
        LoRa.endPacket();              // finish packet and send it
    }

        int getPacketRssi()
    {
        return LoRa.packetRssi();
    }

    String readMessage()
    {
        int packetSize = LoRa.parsePacket();
        String message = "";
        if (packetSize)
        {
            Serial.println("Received message: " + String(packetSize) + " ");
            // read packet header bytes:
            int recipient = LoRa.read();       // recipient address
            byte sender = LoRa.read();         // sender address
            byte incomingMsgId = LoRa.read();  // incoming msg ID
            byte incomingLength = LoRa.read(); // incoming msg length

            if (recipient != _localAddress && recipient != 0xBB)
            {
                Serial.print("This message is not for me, recipient: " + String(recipient) + " ");
                return "NULL";
            }
            while (LoRa.available())
            {
                message += (char)LoRa.read();
            }
            Serial.print(message);
        }
        else
        {
            return "NULL";
        }
        Serial.println(" RSSI: " + String(LoRa.packetRssi()));
        return message;
    }

}
