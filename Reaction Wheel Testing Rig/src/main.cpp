#include <Arduino.h>
#include "motor_wrapper.h"
#include "imu_wrapper.h"
#include "server_wrapper.h"
#include "pid_wrapper.h"

float dWheelSpeed = 0, platSpeed = 0, wheelSpeed = 0, wheelSpeedPrev = 0;

PIDController controler;

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting Reaction Wheel Test");

	controler.Kp = 0.1;
	controler.Ki = 0.1;
	controler.Kd = 0.1;
	controler.tau = 0.95;
	controler.T = 0.02; // sample time in sec
	controler.limMax = 0.95;
	controler.limMin = -0.95;

	pidcontrol::setup(controler);

	motor::setup();
	imu::setup();
}

void loop()
{
	// set PID gains


	imu::readSensor();
	platSpeed = imu::getGyrZ();
	dWheelSpeed = pidcontrol::update(controler, 0, platSpeed);
	wheelSpeed = pidcontrol::increment(wheelSpeed, dWheelSpeed);

	motor::stabilize(wheelSpeed);

	delay(20);
}