#include <Arduino.h>
#include "lora_126x_wrapper.h"
#include "gps_wrapper.h"

int syncTime = 0;

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
	syncTime = gps::getSyncTime();
	lora::executeOperation(syncTime); // execute operation based on what time it is

}