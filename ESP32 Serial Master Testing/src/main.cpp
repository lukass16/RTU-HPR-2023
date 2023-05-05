#include <Arduino.h>
#include "serial_comms.h"

byte data[2] = {0x27, 0xAA};

void setup()
{
	Serial.begin(115200);

	//* Setting up Serial comms
	serialcomms::setup();
}

void loop()
{
	while(!serialcomms::sendCommand(0x07))
	{
		delay(50);
	}
	Serial.println("Command sent successfully");
	delay(500);
}