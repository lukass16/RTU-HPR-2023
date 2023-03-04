#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "asyncserver_wrapper.h"
#include "sdcard_wrapper.h"
#include "pid_wrapper.h"

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

	// setup SD writing
	sdcard::setup();
	fileSD = sdcard::openFile();
	sdcard::writeHeader(fileSD);
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

	// save cycle to SD card
	sdcard::writeData(fileSD, imu::getGyrZ(), wheelSpeed, controler.Kp, controler.Ki, controler.Kd, controler.proportional, controler.integrator, controler.differentiator);

	// * Testing
	Serial.println("Wheel rotational frequency: " + String(motor::getRotationalFrequency()) + " Hz");

	delay(50);	
}