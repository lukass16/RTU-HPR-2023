#pragma once

#include "Arduino.h"
#include "core/core.cpp"
#include "SD_card_log.h"
#include "buzzer_log.h"
#include "gps_wrapper_log.h"
#include "barometer_wrapper_MS5607_log.h"
#include "imu_wrapper_BNO055_log.h"

//*Temporary variables
bool clearEEPROM = true;

class PreperationState : public State
{
public:
    void start() override
    {
        Serial.println("STARTING PARACHUTE TEST DATA LOGGING");

        int data_delay = 50; // delay used in logging [ms]
        int interval = 100; // amount of loops after which the SD file is closed and opened
        int loop_counter = 1;

        // variables for writing to memory
        sens_data::GpsData gd;
        sens_data::BarometerData bd;
        sens_data::IMUData md;
        sens_data::BatteryData btd;

        //*Buzzer setup
        buzzer::setup();

        //*Sensor setups
        Wire.begin(21, 22); // initialize correct i2c lines
        gps::setup();
        barometer::setup();
        imu::setup();

        //*SD card setup
        SD_File fileSD;
        SDcard::setup(); // setup SD card
        fileSD = SDcard::openNextFile();
        SDcard::writeHeader(fileSD); // write header to SD card

        while (true)
        {
            buzzer::signalMain();

            //*gps
            gps::readGps();
            gd = gps::getGpsState();
            s_data.setGpsData(gd);

            //*imu
            imu::readSensor();
            md = imu::getIMUState();
            s_data.setIMUData(md);

            //*barometer
            barometer::readSensor();
            bd = barometer::getBarometerState();
            s_data.setBarometerData(bd);

            // write to SD card
            SDcard::writeSensorData(fileSD, gd, md, bd, true);
            if(loop_counter % interval == 0)
            {
                fileSD = SDcard::closeOpen(fileSD);
            }

            loop_counter++;
            delay(data_delay);
        }
    }

    void HandleNextPhase() override
    {
        Serial.println("End of data logging");
    }
};