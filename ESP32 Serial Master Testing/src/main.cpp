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
	byte response = 0x00;
	while(response != RESPONSE_BYTE)
	{
		serialcomms::sendCommand(0x01);
		delay(100);
		int len = serialcomms::readPacket(true);
		if(len == 1)
		{
			response = serialcomms::readCommand();
		}
	}
	delay(2000);
}