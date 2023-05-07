#include <Arduino.h>
#include "states/preparation_state.cpp"
#include "states/idle_state.cpp"
#include "states/stabilization_state.cpp"
#include "states/pointing_state.cpp"

void setup()
{
	Serial.begin(115200);

	PreparationState().start();
}

void loop()
{
	IdleState().start();
	StabilizationState().start();
	PointingState().start();
}