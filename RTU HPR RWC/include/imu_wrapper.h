#pragma once
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Possible vector values can be:
// - VECTOR_ACCELEROMETER - m/s^2
// - VECTOR_MAGNETOMETER  - uT
// - VECTOR_GYROSCOPE     - rad/s
// - VECTOR_EULER         - degrees
// - VECTOR_LINEARACCEL   - m/s^2
// - VECTOR_GRAVITY       - m/s^2

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);
sensors_event_t angVelocityData, accelerometerData, orientationData;
sensors_event_t *gyrEvent, *accEvent, *orEvent;

namespace imu
{
    float acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z, or_x, or_y, or_z, tmp;

    // variables for platform orientation
    float dor_x = 0, prev_or_x = 0, platOrientation = -1;

    void setup()
    {
        Serial.println("Initializing BNO055");
        /* Initialise the sensor */
        if (!bno.begin(OPERATION_MODE_NDOF))
        {
            /* There was a problem detecting the BNO055 ... check your connections */
            Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
            while (1);
        }
    }

    void readSensor()
    {
        bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
        accEvent = &accelerometerData;
        gyrEvent = &angVelocityData;
        orEvent = &orientationData;
        acc_x = accEvent->acceleration.x / 9.807; //converting to [g]
        acc_y = accEvent->acceleration.y / 9.807;
        acc_z = accEvent->acceleration.z / 9.807;
        gyr_x = gyrEvent->gyro.x;
        gyr_y = gyrEvent->gyro.y;
        gyr_z = gyrEvent->gyro.z;
        or_x = orEvent->orientation.x; // orientation of the sensor with respect to earth and its magnetic field
        or_y = orEvent->orientation.y;
        or_z = orEvent->orientation.z;
    }

    void printAll()
    {
        Serial.print("Acc X: " + String(acc_x, 2));
        Serial.print(", ");
        Serial.print("Acc Y: " + String(acc_y, 2));
        Serial.print(", ");
        Serial.print("Acc Z: " + String(acc_z, 2));

        Serial.print("\t");
        Serial.print("Gyro X: " + String(gyr_x, 2));
        Serial.print(", ");
        Serial.print("Gyro Y: " + String(gyr_y, 2));
        Serial.print(", ");
        Serial.println("Gyro Z: " + String(gyr_z, 2));
    }

    void printCalibrationStatus()
    {
        uint8_t system, gyro, accel, mag = 0;
        bno.getCalibration(&system, &gyro, &accel, &mag);
        Serial.println();
        Serial.print("Calibration: Sys=");
        Serial.print(system);
        Serial.print(" Gyro=");
        Serial.print(gyro);
        Serial.print(" Accel=");
        Serial.print(accel);
        Serial.print(" Mag=");
        Serial.println(mag);
    }

    void printMode()
    {
        Serial.print("Mode: ");
        Serial.println(bno.getMode());
    }

    void printSystemStatus()
    {
        uint8_t status, test_res, err;
        bno.getSystemStatus(&status, &test_res, &err);
        Serial.print("Status: ");
        Serial.print(status);
        Serial.print("\tSelf Test Result: ");
        Serial.print(test_res);
        Serial.print("\tSystem Error: ");
        Serial.println(err);
    }


    float getGyrX()
    {
        return gyr_x;
    }

    float getGyrY()
    {
        return gyr_y;
    }

    float getGyrZ()
    {
        return -gyr_z; // sign changed for controller purposes
    }

    float getOrX() // direction of platform (effective yaw)
    {
        return or_x;
    }

    float getOrY()
    {
        return or_y;
    }

    float getOrZ()
    {
        return or_z;
    }

    float getRPM()
    {
        return gyr_z * 60 / 2 / PI; // converting from radians/s to rotations/min
    }

    float setPlatOrientation(float _platOrientation)
    {
        platOrientation = _platOrientation;
        return platOrientation;
    }

    float getPlatOrientation()
    {
        static bool firstFlag = 1;

        dor_x = or_x - prev_or_x;

        // handle jump cases
        if(dor_x > 330)
        {
            dor_x -= 360;
        }
        else if(dor_x < -330)
        {
            dor_x += 360;
        }

        if(firstFlag) // for first call just setup the function
        {
            dor_x = 0;
            firstFlag = 0;
        }
        platOrientation += dor_x; // add the change in orientation to the platform orientation

        prev_or_x = or_x; 
        return platOrientation;
    }

    void bruteForceCalibration()
    {
        int systemCalCount = 0;
        uint8_t system, gyro, accel, mag = 0;
        bno.getCalibration(&system, &gyro, &accel, &mag);
        while (systemCalCount < 20)
        {
            // display calibration values
            bno.getCalibration(&system, &gyro, &accel, &mag);
            Serial.println();
            Serial.print("Calibration: Sys=");
            Serial.print(system);
            Serial.print(" Gyro=");
            Serial.print(gyro);
            Serial.print(" Accel=");
            Serial.print(accel);
            Serial.print(" Mag=");
            Serial.println(mag);

            // update system calibration count
            if(system == 3)
            {
                systemCalCount++;
            }
            else
            {
                systemCalCount = 0;
            }

            // display orientation in x axis
            imu::readSensor();
            Serial.println("Or x: " + String(or_x, 2));
            delay(100);
        }

        // set initial orientation
        imu::readSensor();
        imu::setPlatOrientation(or_x);
    }
}
