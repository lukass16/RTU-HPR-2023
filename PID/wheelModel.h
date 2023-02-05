#ifndef TEST_MODEL_H
#define TEST_MODEL_H

typedef struct {
	float angleSpeed;
	float inertia;
	float maxAnglSpeed;
	float maxAnglSpeedChange;
}wheelModel;

typedef struct{
	float initAngleSpeed;
	float angleSpeed;
	float inertia;
}platformModel;

void updateWheel(wheelModel *wheel, float speedDiff);
void updatePlatform(platformModel *platform, wheelModel *wheel);
float platformToWheelSpeed(platformModel* platform, wheelModel* wheel, float plSpeed);

#endif
