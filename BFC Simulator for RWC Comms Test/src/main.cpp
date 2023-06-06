#include <Arduino.h>
#include "lora_126x_wrapper.h"
#include "serial_comms.h"

void setup()
{
	Serial.begin(115200);
	lora::setup();

	serialcomms::setup();
	delay(1000);
}

void loop()
{
	//lora::send("Gerard", 1);
	uint8_t command = lora::receiveCommand();
	if (command != 0)
	{
		while (!serialcomms::sendCommand(command, 1))
		{
			delay(50);
		}
		Serial.println("Command sent");
	}
}