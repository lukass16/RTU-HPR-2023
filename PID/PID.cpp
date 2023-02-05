#include "PID.h"

void PIDController_Init(PIDController* pid) {
	pid->integrator = 0.0f;
	pid->prevError = 0.0f;

	pid->differentiator = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;
}

float PIDController_Update(PIDController* pid, float setpoint, float measurement) {

	float error = setpoint - measurement;

	float proportional = pid->Kp * error;

	pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	//add clamping logic

	float limMinInt, limMaxInt;

	if (pid->limMax > proportional) {

		limMaxInt = pid->limMax - proportional;
	
	} 
	else {

		limMaxInt = 0.0f;
	}


	if (pid->limMin < proportional) {

		limMinInt = pid->limMin - proportional;

	}
	else {

		limMinInt = 0.0f;

	}

	if (pid->integrator > limMaxInt) {

		pid->integrator = limMaxInt;
	}
	else if (pid->integrator < limMinInt){

		pid->integrator = limMinInt;

	}

	pid->differentiator = (2.0f * pid->Kd * (measurement - pid->prevMeasurement)
						+ (2.0f * pid->tau - pid->T) * pid->differentiator)
						/ (2.0f * pid->tau + pid->T);

	pid->out = proportional + pid->integrator + pid->differentiator;

	if (pid->out > pid->limMax) {

		pid->out = pid->limMax;

	}
	else if (pid->out < pid->limMin) {

		pid->out = pid->limMin;

	}

	pid->prevError = error;
	pid->prevMeasurement = measurement;

	return pid->out;
}