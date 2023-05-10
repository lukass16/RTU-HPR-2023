#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "asyncserver_wrapper.h"
#include "sdcard_wrapper.h"
#include "pid_wrapper.h"
#include "buzzer.h"

//* Testing
unsigned long prev_t = millis();

float wheelSpeed = 0;

PIDMode mode = STABILIZE;

SD_File fileSD;

void setup()
{
	Serial.begin(115200);

	// indicate start
	buzzer::setup();
	Serial.println("Starting Reaction Wheel Test");

	asyncserver::setup();	
	motor::setup();
	motor::setupEncoder();
	imu::setup();

	// setup PID
	pidcontrol::setup();

	// calibrate sensor
	imu::bruteForceCalibration();

	// warn of start
	buzzer::signalStartWarning();
}

void loop()
{
	// // if data received, set new PID gains
	//* Needs to be updated
	// if(asyncserver::hasData())
	// {
	// 	controler.Kp = asyncserver::getP();
	// 	controler.Ki = asyncserver::getI();
	// 	controler.Kd = asyncserver::getD();
	// 	asyncserver::printGains();
	// }

	//* Changing modes for test
	if(mode == STABILIZE)
	{
		imu::readSensor();
		wheelSpeed = pidcontrol::update(0);
	}
	else if(mode == POINT)
	{
		imu::readSensor();
		wheelSpeed = pidcontrol::update(160);
	}
	if(millis() - prev_t > 10000)
	{
		prev_t = millis();
		if(mode == STABILIZE)
		{
			mode = POINT;
			pidcontrol::setMode(mode);
			buzzer::signalPointMode();
		}
		else if(mode == POINT)
		{
			mode = STABILIZE;
			pidcontrol::setMode(mode);
			buzzer::signalStabilizeMode();
		}
	}

	
	//* Testing
	pidcontrol::printMode();
	Serial.println("Wheel Speed: " + String(wheelSpeed, 2));
	Serial.println("Plat Angular Velocity: " + String(imu::getGyrZ(), 2));
	Serial.println("Plat Orientation: " + String(imu::getPlatOrientation(), 2));
	pidcontrol::printTerms();


	motor::setWheelSpeed(wheelSpeed);

	delay(50);	
}