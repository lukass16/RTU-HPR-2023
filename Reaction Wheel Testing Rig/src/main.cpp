#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "server_wrapper.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Reaction Wheel Test");
	motor::setup();

	imu::setup();

	// Starting motor movement
	motor::setDirection(1);
	motor::setDutyCycle(30);

	// Starting server
	server::setup();
}

void loop()
{
	imu::readSensor();
	delay(1000);
	Serial.println(imu::getGyrX());
	server::handleClient();
	server::setData();
}