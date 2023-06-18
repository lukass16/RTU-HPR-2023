#include <Arduino.h>
#include "lora_126x_wrapper.h"
#include "gps_wrapper.h"

void setup()
{
	Serial.begin(115200);
	lora::setup();
	
	gps::setup();
}

void loop()
{
	//lora::pingPong();
	gps::readGps();
	gps::printTime();
}