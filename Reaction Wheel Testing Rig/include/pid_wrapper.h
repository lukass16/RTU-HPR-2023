#pragma once

struct PIDController
{

	float Kp;
	float Ki;
	float Kd;

	float tau;

	float limMin;
	float limMax;

	float T; // sample time in sec

	float integrator;
	float prevError;
	float differentiator;
	float prevMeasurement;

	float out;
};

namespace pidcontrol
{
	void setup(PIDController* pid)
	{
		pid->integrator = 0.0f;
		pid->prevError = 0.0f;

		pid->differentiator = 0.0f;
		pid->prevMeasurement = 0.0f;

		pid->out = 0.0f;
	}

	float update(PIDController* pid, float setpoint, float measurement)
	{
		float error = setpoint - measurement;

		//* Proportional component calculations
		float proportional = pid->Kp * error;

		//* Integral component calculations
		pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

		//* Integral clamping logic
		float limMinInt, limMaxInt;

		if (pid->limMax > proportional)
		{
			limMaxInt = pid->limMax - proportional;
		}
		else
		{
			limMaxInt = 0.0f;
		}

		if (pid->limMin < proportional)
		{
			limMinInt = pid->limMin - proportional;
		}
		else
		{
			limMinInt = 0.0f;
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
		pid->differentiator = (2.0f * pid->Kd * (measurement - pid->prevMeasurement) + (2.0f * pid->tau - pid->T) * pid->differentiator) / (2.0f * pid->tau + pid->T);

		//* Total output calculations
		pid->out = proportional + pid->integrator + pid->differentiator;

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
		pid->prevMeasurement = measurement;

		//Serial.println("Err: " + String(error));
		Serial.println("P:"+String(proportional)+" I:"+String(pid->integrator)+" D:"+String(pid->differentiator));

		return pid->out;
	}

};