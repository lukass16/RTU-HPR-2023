#pragma once
#include "imu_wrapper.h"

enum PIDMode
{
	STABILIZE,
	POINT
};

struct PIDController
{
	PIDMode mode;

	float Kp;
	float Ki;
	float Kd;

	float tau;

	float limMin;
	float limMax;

	float T; // sample time in sec

	float proportional;
	float integrator;
	float prevError;
	float differentiator;
	float measurement;
	float prevMeasurement;

	float out;
};

namespace pidcontrol
{
	void setup(PIDController *pid)
	{
		pid->mode = STABILIZE;

		// setup controller with stabilizing gains
		pid->Kp = 0.9;
		pid->Ki = 5.0;
		pid->Kd = 0.04;
		pid->tau = 0.9;
		pid->T = 0.05; // sample time in sec
		pid->limMax = 0.95;
		pid->limMin = -0.95;

		pid->proportional = 0.0f;
		pid->integrator = 0.0f;
		pid->prevError = 0.0f;

		pid->differentiator = 0.0f;
		pid->prevMeasurement = 0.0f;

		pid->out = 0.0f;
	}

	float update(PIDController *pid, float setpoint)
	{
		//* automatically retrieve required measurement based on mode
		if (pid->mode == STABILIZE)
		{
			pid->measurement = imu::getGyrZ(); // measure angular velocity
		}
		else if (pid->mode == POINT)
		{
			pid->measurement = imu::getPlatOrientation(); // measure platform orientation (custom variable)
		}

		float error = setpoint - pid->measurement;

		//* Proportional component calculations
		pid->proportional = pid->Kp * error;

		//* Integral component calculations
		pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

		//* Integral clamping logic
		float limMinInt, limMaxInt;

		if (pid->limMax > pid->proportional) // if the proportional term is less than the output upper limit
		{
			limMaxInt = pid->limMax - pid->proportional; // the limit of the integrator term is what's left up to the maximum
		}
		else
		{
			limMaxInt = 0.0f; // else we zero the integral term
		}

		if (pid->limMin < pid->proportional) // if the proportional term is more than the output lower limit
		{
			limMinInt = pid->limMin - pid->proportional; // the limit of the integrator term is what's left down to the minimum
		}
		else
		{
			limMinInt = 0.0f; // else we zero the integral term
		}

		if (pid->integrator > limMaxInt)
		{
			pid->integrator = limMaxInt;
		}
		else if (pid->integrator < limMinInt)
		{
			pid->integrator = limMinInt;
		}

		//* Derivative component calculations
		if (pid->mode == STABILIZE)
		{
			pid->differentiator = (2.0f * pid->Kd * (pid->measurement - pid->prevMeasurement) + (2.0f * pid->tau - pid->T) * pid->differentiator) / (2.0f * pid->tau + pid->T);
		}
		else if (pid->mode == POINT)
		{
			pid->differentiator = pid->Kd * (imu::getGyrZ() * 180 / PI);
		}

		//* Total output calculations
		pid->out = pid->proportional + pid->integrator + pid->differentiator;

		//* Output clamping
		if (pid->out > pid->limMax)
		{
			pid->out = pid->limMax;
		}
		else if (pid->out < pid->limMin)
		{
			pid->out = pid->limMin;
		}

		pid->prevError = error;
		pid->prevMeasurement = pid->measurement;

		return pid->out;
	}

	void setMode(PIDController *pid, PIDMode mode)
	{
		if (mode == STABILIZE)
		{
			pid->Kp = 0.9;
			pid->Ki = 5.0;
			pid->Kd = 0.04;

			// keep only the integral term
			pid->proportional = 0;
			pid->differentiator = 0;
			pid->prevMeasurement = imu::getGyrZ();
		}
		else if (mode == POINT)
		{
			pid->Kp = 0.003;
			pid->Ki = 0.0005;
			pid->Kd = -0.002;

			// keep only the integral term
			pid->proportional = 0;
			pid->differentiator = 0;
			pid->prevMeasurement = imu::getPlatOrientation();
		}
		pid->mode = mode;
	}

	//* Testing purposes
	void printMode(PIDController *pid)
	{
		if(pid->mode == STABILIZE)
		{
			Serial.println("Mode: STABILIZE");
		}
		else if(pid->mode == POINT)
		{
			Serial.println("Mode: POINT");
		}
	}
	void printTerms(PIDController *pid)
	{
		Serial.println("P:" + String(pid->proportional) + " I:" + String(pid->integrator) + " D:" + String(pid->differentiator));
	}
};