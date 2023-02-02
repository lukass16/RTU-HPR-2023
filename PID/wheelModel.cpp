#include "wheelModel.h"
#include <cmath>

void updateWheel(wheelModel* wheel, float speedDiff) {

	if (wheel->maxAnglSpeedChange > abs(speedDiff) && wheel->maxAnglSpeed > abs(wheel->angleSpeed + speedDiff)) {

		wheel->angleSpeed = wheel->angleSpeed + speedDiff;
	
	}
	else if (wheel->maxAnglSpeedChange < abs(speedDiff) && wheel->maxAnglSpeed > abs(wheel->angleSpeed + speedDiff)) {

		wheel->angleSpeed = wheel->angleSpeed + wheel->maxAnglSpeedChange * (speedDiff/ abs(speedDiff));
	
	}
	else if (wheel->maxAnglSpeed < abs(wheel->angleSpeed + speedDiff)) {

		wheel->angleSpeed = wheel->maxAnglSpeed;
	
	}
}

void updatePlatform(platformModel* platform, wheelModel* wheel) {

	platform->angleSpeed = platform->initAngleSpeed + (wheel->angleSpeed * wheel->inertia) / platform->inertia;

}

float platformToWheelSpeed(platformModel* platform, wheelModel* wheel, float plSpeed) {
	return (platform->inertia * plSpeed) / wheel->inertia;
}