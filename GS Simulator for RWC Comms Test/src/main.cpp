#include <Arduino.h>
#include "serial_comms.h"
#include "lora_wrapper.h"

byte data[2] = {0x27, 0xAA};

void setup()
{
	Serial.begin(115200);

	//* Setting up Serial comms
	serialcomms::setup();
	delay(1000);

	//* Setting up LoRa
	lora::setup();
	delay(1000);
}

void loop()
{
	
	// read inputs ferom serial
	u_int8_t command = serialcomms::getHexCommandFromSerial(true);
	if(command != 0)
	{
		lora::sendByte(command);
		Serial.println("Sent HEX command: " + String(command, HEX));
	}

}