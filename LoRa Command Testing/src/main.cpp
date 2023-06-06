#include <Arduino.h>
#include "lora_126x_wrapper.h"

void setup()
{
	Serial.begin(115200);
	lora::setup();
}

void loop()
{
	//lora::send("Gerard", 1);
	lora::receiveCommand();
}