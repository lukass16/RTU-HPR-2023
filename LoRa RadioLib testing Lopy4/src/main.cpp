// include the library
#include <Arduino.h>
#include <RadioLib.h>

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

SX1276 radio = new Module(SS, DIO0, RST, 23);

// or using RadioShield
// https://github.com/jgromes/RadioShield
// SX1278 radio = RadioShield.ModuleA;

void setup()
{
  Serial.begin(115200);

  // SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin(434, 125, 9, 7, 0x12, 10);
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
  //Serial.print(F("[SX1278] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  String str;
  int state = radio.receive(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE)
  {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[SX1278] Data:\t\t\t"));
    Serial.println(str);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[SX1278] RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("[SX1278] SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    // of the last received packet
    Serial.print(F("[SX1278] Frequency error:\t"));
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