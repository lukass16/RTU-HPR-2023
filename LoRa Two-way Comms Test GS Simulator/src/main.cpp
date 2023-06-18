#include <Arduino.h>
#include "lora_127x_wrapper.h"
#include "gps_wrapper.h"

#define GPS_TIME_TRIGGER 2

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
