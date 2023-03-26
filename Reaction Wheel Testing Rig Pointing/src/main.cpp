#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "asyncserver_wrapper.h"
#include "sdcard_wrapper.h"
#include "pid_wrapper.h"
#include "buzzer.h"

float platOrientation = 0, wheelSpeed = 0;

PIDController controler;

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

	controler.Kp = 0.001; //* Notes while testing: 0.003-0.01 seems like an ok sensitivity range for solely P term
	controler.Ki = 0.0001;
	controler.Kd = -0.0006;
	controler.tau = 0.9;
	controler.T = 0.05; // sample time in sec
	controler.limMax = 0.65; //* Notes while testing: 0.3-0.5 seems like an ok range for the limit values
	controler.limMin = -0.65; 

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

	imu::readSensor();
	platOrientation = imu::getPlatOrientation(); // using as the process variable our own platform orientation
	wheelSpeed = pidcontrol::update(&controler, 160, platOrientation);
	//* Testing
	Serial.println("Wheel Speed: " + String(wheelSpeed, 2));
	Serial.println("Plat Orientation: " + String(platOrientation, 2));
	pidcontrol::printTerms(&controler);


	motor::stabilize(wheelSpeed);

	delay(50);	
}