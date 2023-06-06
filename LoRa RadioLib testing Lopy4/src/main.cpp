// // include the library
#include <Arduino.h>
#include <RadioLib.h>
#include "lora_rswrapper.h"

void setup()
{
	Serial.begin(115200);
	lora::setup();
}

void loop()
{
	lora::sendByte(0x34);

	delay(1000);
}
