#include <Arduino.h>
#include "serial_comms.h"

byte data[2] = {0x27, 0xAA};

void setup()
{
	Serial.begin(115200);

	//* Setting up Serial comms
	serialcomms::setup();
	delay(1000);
}

void loop()
{
	// while(!serialcomms::sendCommand(GO_IDLE))
	// {
	// 	delay(50);
	// }
	// delay(2000);
	// while(!serialcomms::sendCommand(START_STABILIZATION))
	// {
	// 	delay(50);
	// }
	// delay(2000);
	// while(!serialcomms::sendCommand(START_POINTING))
	// {
	// 	delay(50);
	// }
	// delay(2000);

	// read inputs ferom serial
	u_int8_t command = serialcomms::getHexCommandFromSerial(true);
	if (command != 0)
	{
		while (!serialcomms::sendCommand(command))
		{
			delay(50);
		}
		Serial.println("Command sent");
	}

}