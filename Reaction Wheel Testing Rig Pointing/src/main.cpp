#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "asyncserver_wrapper.h"
#include "sdcard_wrapper.h"
#include "pid_wrapper.h"

float platOrientation = 0, wheelSpeed = 0;

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

	controler.Kp = 0.03;
	controler.Ki = 0.01;
	controler.Kd = 0.005;
	controler.tau = 0.9;
	controler.T = 0.05; // sample time in sec
	controler.limMax = 0.95;
	controler.limMin = -0.95;

	pidcontrol::setup(&controler);

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
	wheelSpeed = pidcontrol::update(&controler, 90, platOrientation);
	Serial.println("Wheel Speed: " + String(wheelSpeed, 2));
	Serial.println("Plat Orientation: " + String(platOrientation, 2));

	motor::stabilize(wheelSpeed);

	delay(50);	
}