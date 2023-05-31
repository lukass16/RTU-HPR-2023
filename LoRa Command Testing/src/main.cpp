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

/*
   RadioLib SX126x Transmit Example
   This example transmits packets using SX1262 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)
   Other modules from SX126x family can also be used.
   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem
   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// SX1262 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9


// or using RadioShield
// https://github.com/jgromes/RadioShield
// SX1262 radio = RadioShield.ModuleA;

// or using CubeCell
// SX1262 radio = new Module(RADIOLIB_BUILTIN_MODULE);

void setup()
{
  Serial.begin(115200);


}

void loop()
{
  
  int state = radio.transmit("Lullaby");  // can transmit C-string or Arduino string up to 256 characters long

  // wait for a second before transmitting again
  delay(1000);
}