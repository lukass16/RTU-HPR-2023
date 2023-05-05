#include <Arduino.h>
#include "serial_comms.h"

float testFloat = 2.342;
byte response = 0x00;
byte test = 0x00;

byte data[2] = {0x27, 0xAA};

void setup()
{
	Serial.begin(115200);

	//* Setting up Serial comms
	serialcomms::setup();
}

void loop()
{
	serialcomms::sendPacket(data, 2);
	delay(1000);
}