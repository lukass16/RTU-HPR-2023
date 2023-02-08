#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "asyncserver_wrapper.h"
#include "pid_wrapper.h"

float dWheelSpeed = 0, platSpeed = 0, wheelSpeed = 0, wheelSpeedPrev = 0;

PIDController controler;

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Reaction Wheel Test");

	asyncserver::setup();
	motor::setup();
	imu::setup();

	controler.Kp = 0.8;
	controler.Ki = 0.2;
	controler.Kd = 0.2;
	controler.tau = 0.90;
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
	platSpeed = imu::getGyrZ();
	wheelSpeed = pidcontrol::update(&controler, 0, platSpeed);

	motor::stabilize(wheelSpeed);

	delay(50);
}