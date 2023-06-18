#include <Arduino.h>
#include "lora_126x_wrapper.h"
#include "gps_wrapper.h"

#define GPS_TIME_TRIGGER 0

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
	if(gps::getSyncTime() != syncTime)
	{
		syncTime = gps::getSyncTime();
		Serial.println("Sync time: " + String(syncTime));
		if(syncTime == GPS_TIME_TRIGGER)
		{
			Serial.println("SEND triggered.");
		}
	}
	
}