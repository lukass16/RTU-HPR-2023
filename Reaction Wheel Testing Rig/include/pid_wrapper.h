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

    float proportional;
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
		pid->proportional = 0.0f;
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
		pid->differentiator = (2.0f * pid->Kd * (measurement - pid->prevMeasurement) + (2.0f * pid->tau - pid->T) * pid->differentiator) / (2.0f * pid->tau + pid->T);

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
		pid->prevMeasurement = measurement;

		//Serial.println("Err: " + String(error));
		//Serial.println("P:"+String(pid->proportional)+" I:"+String(pid->integrator)+" D:"+String(pid->differentiator));

		return pid->out;
	}

};