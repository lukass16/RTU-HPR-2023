#pragma once
#include <Arduino.h>
#include "mutex_wrapper_log.h"

namespace sens_data
{

    struct GpsData
    {
        double lat = 0;
        double lng = 0;
        double alt = 0;
        int sats = 0;
    };

    struct IMUData
    {
        // Acceleration
        float acc_x = 0;
        float acc_y = 0;
        float acc_z = 0;
        // Rotation
        float gyr_x = 0;
        float gyr_y = 0;
        float gyr_z = 0;
        // Magnetic field
        float mag_x = 0;
        float mag_y = 0;
        float mag_z = 0;
        // Orientation
        float or_x = 0;
        float or_y = 0;
        float or_z = 0;

    };

    struct BarometerData
    {
        float temperature = 0;
        float pressure = 0;
        float altitude = 0;
        float f_altitude = 0;
        float f_velocity = 0;     // vertical velocity
        float f_acceleration = 0; // vertical acceleration
    };

    struct BatteryData
    {
        float bat1 = 0;
        float bat2 = 0;
        int bs = 0;
    };

    class SensorData
    {
    private:
        int current_rocket_state = 0; // 1 - PREPARATION, 2 - LAUNCH WAIT, 3 - APOGEE WAIT, 4 - MAIN WAIT, 5 - DESCENT, 6 TOUCHDOWN
        GpsData gpsData;
        IMUData imuData;
        BarometerData barometerData;
        BatteryData batteryData;
        Mtx mtx;

    public:
        byte lora_message_id = 0;

        SensorData()
        {
            mtx.setup();
        };

        void setRocketState(int rs)
        {
            mtx.lock();
            current_rocket_state = rs;
            mtx.unlock();
        }

        int getRocketState()
        {
            mtx.lock();
            int state = current_rocket_state;
            mtx.unlock();
            return state;
        }

        IMUData getIMUData()
        {
            mtx.lock();
            IMUData md = imuData;
            mtx.unlock();
            return md;
        }
        void setIMUData(IMUData md)
        {
            mtx.lock();
            imuData = md;
            mtx.unlock();
        }

        GpsData getGpsData()
        {
            mtx.lock();
            GpsData gd = gpsData;
            mtx.unlock();
            return gd;
        }
        void setGpsData(GpsData gd)
        {
            mtx.lock();
            gpsData = gd;
            mtx.unlock();
        }

        BarometerData getBarometerData()
        {
            mtx.lock();
            BarometerData bd = barometerData;
            mtx.unlock();
            return bd;
        }
        void setBarometerData(BarometerData bd)
        {
            mtx.lock();
            barometerData = bd;
            mtx.unlock();
        }

        BatteryData getBatteryData()
        {
            mtx.lock();
            BatteryData btd = batteryData;
            mtx.unlock();
            return btd;
        }
        void setBatteryData(BatteryData btd)
        {
            mtx.lock();
            batteryData = btd;
            mtx.unlock();
        }
    };

}

extern sens_data::SensorData s_data;