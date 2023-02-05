#include <iostream>
#include "PID.h"
#include "wheelModel.h"

using namespace std;

int main()
{
    PIDController ctrl_obj;
    ctrl_obj.Kp = 0.5f;
    ctrl_obj.Ki = 0.5f;
    ctrl_obj.Kd = 0.2f;
    ctrl_obj.tau = 0.9f;
    ctrl_obj.T = 0.02f; // sample time in sec
    ctrl_obj.limMax = 0.9;
    ctrl_obj.limMin = -0.9;

    wheelModel wh;
    wh.angleSpeed = 0;
    wh.maxAnglSpeed = 2000;
    wh.maxAnglSpeedChange = 10; //rad*s*s
    wh.inertia = 10;

    platformModel pl;
    pl.initAngleSpeed = 10;
    pl.angleSpeed = pl.initAngleSpeed;
    pl.inertia = 100;

    PIDController_Init(&ctrl_obj);

    //while (pl.angleSpeed > 0) {
    for(int i = 0; i < 100; i++){
        float dPlatSpeed = PIDController_Update(&ctrl_obj, 0, pl.angleSpeed);
        float dWheelSpeed = platformToWheelSpeed(&pl, &wh, dPlatSpeed);
        updateWheel(&wh, dWheelSpeed);

        updatePlatform(&pl, &wh);
        cout << pl.angleSpeed << "\n";
    }

    cout << "Wheel speed : " << wh.angleSpeed << "\n";
    cout << "Hello World!\n";
}
