#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "server_wrapper.h"
int i = 1;

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Reaction Wheel Test");
	motor::setup();

	imu::setup();

	// Starting motor movement
	motor::setDirection(1);
	motor::setDutyCycle(150);

	// Starting server
	// server::setup();
}

void loop()
{
	// delay(500);
	// imu::readSensor();
	// if (i)
	// {
	// 	motor::setDirection(0);
	// 	i = 0;
	// }
	// else
	// {
	// 	motor::setDirection(1);
	// 	i = 1;
	// }

	Serial.println("X: " + String(imu::getGyrX()));
	Serial.println("Y: " + String(imu::getGyrY()));
	Serial.println("Z: " + String(imu::getGyrZ()));
	// server::handleClient();
	// server::setData();
}