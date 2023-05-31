// // include the library
#include <Arduino.h>
#include <RadioLib.h>

#define SS 18
#define RST 14
#define DIO0 26
#define DIO1 35
#define SCK 5
#define MISO 19
#define MOSI 27

SX1276 radio = new Module(SS, DIO0, RST, DIO1);

void setup()
{
  Serial.begin(115200);

  // SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(434, 125, 9, 7, 0x34, 10);
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}

void loop()
{

  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE)
  {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[SX1276] Data:\t\t\t"));
    Serial.println(str);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[SX1276] RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("[SX1276] SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    // of the last received packet
    Serial.print(F("[SX1276] Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));
  }
  else if (state == RADIOLIB_ERR_RX_TIMEOUT)
  {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));
  }
  else if (state == RADIOLIB_ERR_CRC_MISMATCH)
  {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}


// //* Receiving messages
// #include <Arduino.h>
// #include "lora_rswrapper.h"

// void setup() {
//   Serial.begin(115200);
//   lora::setup();
// }

// void loop() {
//   lora::readMessage();
// }


//* Channel activity
/*
   RadioLib SX127x Channel Activity Detection Example

   This example scans the current LoRa channel and detects
   valid LoRa preambles. Preamble is the first part of
   LoRa transmission, so this can be used to check
   if the LoRa channel is free, or if you should start
   receiving a message.

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// SX1278 has the following connections:
// NSS pin:   10
// DIO0 pin:  2
// RESET pin: 9
// DIO1 pin:  3
// SX1276 radio = new Module(SS, DIO0, RST, INTR);

// // or using RadioShield
// // https://github.com/jgromes/RadioShield
// //SX1278 radio = RadioShield.ModuleA;

// void setup() {
//   Serial.begin(115200);
//   SPI.begin(SCK, MISO, MOSI, SS);

//   // initialize SX1278 with default settings
//   Serial.print(F("[SX1278] Initializing ... "));
//   int state = radio.begin(434, 125, 9, 7, 0x12, 10, 8);
//   if (state == RADIOLIB_ERR_NONE) {
//     Serial.println(F("success!"));
//   } else {
//     Serial.print(F("failed, code "));
//     Serial.println(state);
//     while (true);
//   }
// }

// void loop() {
//   Serial.print(F("[SX1278] Scanning channel for LoRa preamble ... "));

//   // start scanning current channel
//   int state = radio.scanChannel();

//   if (state == RADIOLIB_PREAMBLE_DETECTED) {
//     // LoRa preamble was detected
//     Serial.println(F("detected preamble!"));

//   } else if (state == RADIOLIB_CHANNEL_FREE) {
//     // no preamble was detected, channel is free
//     Serial.println(F("channel is free!"));

//   }

//   // wait 100 ms before new scan
//   delay(100);
// }