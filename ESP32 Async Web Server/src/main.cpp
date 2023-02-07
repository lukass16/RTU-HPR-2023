#include <Arduino.h>
#include "asyncserver_wrapper.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting ESP32 Async Web Server Test");
	asyncserver::setup();
}

void loop() 
{
	if(asyncserver::hasData())
	{
		Serial.println(asyncserver::getP(), 4);
		Serial.println(asyncserver::getI(), 4);
		Serial.println(asyncserver::getD(), 4);
	}
}