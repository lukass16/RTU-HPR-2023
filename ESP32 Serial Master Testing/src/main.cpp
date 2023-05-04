#include <Arduino.h>
#include "serial_comms.h"

float testFloat = 2.342;

void setup()
{
	Serial.begin(115200);

	//* Setting up Serial comms
	serialcomms::setup();
	serialcomms::prepareDataPacket(testFloat);
}

void loop()
{
	serialcomms::sendPacket(serialcomms::data, 4);
	serialcomms::displayPacket(serialcomms::data, 4, true);
	delay(1000);
	
	testFloat -= 0.300;
	serialcomms::prepareDataPacket(testFloat);
}