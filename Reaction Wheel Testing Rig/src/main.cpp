#include <Arduino.h>
#include "motor.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Reaction Wheel Test");
	motor::setup();
	
	motor::setDirection(1);
	motor::setDutyCycle(30);
}

void loop()
{
	//Serial.println("A: " + String(analogRead(A)));
	// if (duty <= 255)
	// {
	// 	pwm::setDutyCycle(duty);
	// 	Serial.println("Duty cycle set to: " + String(duty) + "/255");
	// }
	// delay(2000);
	// duty += 10;
}