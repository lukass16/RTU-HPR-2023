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

PIDController controler;
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

	controler.Kp = 0.003; 
	controler.Ki = 0.0005;
	controler.Kd = -0.002;
	controler.tau = 0.9;
	controler.T = 0.05; // sample time in sec
	controler.limMax = 0.95; 
	controler.limMin = -0.95; 

	pidcontrol::setup(&controler);

	// calibrate sensor
	imu::bruteForceCalibration();

	// warn of start
	buzzer::signalStartWarning();
}

void loop()
{
	// if data received, set new PID gains
	if(asyncserver::hasData())
	{
		controler.Kp = asyncserver::getP();
		controler.Ki = asyncserver::getI();
		controler.Kd = asyncserver::getD();
		asyncserver::printGains();
	}

	//* Changing modes for test
	if(mode == STABILIZE)
	{
		imu::readSensor();
		wheelSpeed = pidcontrol::update(&controler, 0);
	}
	else if(mode == POINT)
	{
		imu::readSensor();
		wheelSpeed = pidcontrol::update(&controler, 160);
	}
	if(millis() - prev_t > 10000)
	{
		prev_t = millis();
		if(mode == STABILIZE)
		{
			mode = POINT;
			pidcontrol::setMode(&controler, mode);
			buzzer::signalPointMode();
		}
		else if(mode == POINT)
		{
			mode = STABILIZE;
			pidcontrol::setMode(&controler, mode);
			buzzer::signalStabilizeMode();
		}
	}

	
	//* Testing
	pidcontrol::printMode(&controler);
	Serial.println("Wheel Speed: " + String(wheelSpeed, 2));
	Serial.println("Plat Angular Velocity: " + String(imu::getGyrZ(), 2));
	Serial.println("Plat Orientation: " + String(imu::getPlatOrientation(), 2));
	pidcontrol::printTerms(&controler);


	motor::stabilize(wheelSpeed);

	delay(50);	
}