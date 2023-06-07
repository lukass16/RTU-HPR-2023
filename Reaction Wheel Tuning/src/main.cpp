#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "asyncserver_wrapper.h"
#include "sdcard_wrapper.h"
#include "pid_wrapper.h"

/*
The following code is used to test the reaction wheel and the stabilization PID controller
as well as to tune the PID controller and select the relevant PID gains
*/

float platSpeed = 0, wheelSpeed = 0;

PIDController controler;

SD_File fileSD;

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Reaction Wheel Test");

	asyncserver::setup();	
	motor::setup();
	motor::setupEncoder();
	imu::setup();

	controler.Kp = 0.9;
	controler.Ki = 5.0;
	controler.Kd = 0.04;
	controler.tau = 0.9;
	controler.T = 0.05; // sample time in sec
	controler.limMax = 0.95;
	controler.limMin = -0.95;

	pidcontrol::setup(&controler);

	// Motor test 3:
	/*
	setup SD writing
	sdcard::setup();
	fileSD = sdcard::openFile();
	sdcard::writeHeader(fileSD);

	motor::testCountdown();
	while(true)
	{
		motor::test3();
	}
	*/
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
	platSpeed = imu::getGyrZ();
	wheelSpeed = pidcontrol::update(&controler, 0, platSpeed);

	motor::stabilize(wheelSpeed);

	// // save cycle to SD card
	// sdcard::writeData(fileSD, imu::getGyrZ(), wheelSpeed, controler.Kp, controler.Ki, controler.Kd, controler.proportional, controler.integrator, controler.differentiator);

	delay(50);	
}