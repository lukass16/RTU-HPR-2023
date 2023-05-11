#pragma once
#include <SPIFFS.h>
#include <mySD.h>
#include "sensor_data_log.h"

// http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it/
// Inspiration:	default esp32 SD sample code from library ESP-FTP-Server-Lib
// https://github.com/LilyGO/TTGO-T-Beam/issues/9

// thread about LoRa and SD conflict: https://stackoverflow.com/questions/57454066/how-to-use-2-spi-devices-lora-and-sd-card-on-esp32

#define _MISO 13
#define _MOSI 12
#define _SCLK 2
#define _SD_CS 25

unsigned long SD_time = millis();

namespace SDcard
{
	char filename[7] = "0.csv";

	void setup()
	{
		pinMode(_SD_CS, OUTPUT);

		if (SD.begin(_SD_CS, _MOSI, _MISO, _SCLK))
		{
			Serial.println("SD card initialized OK");
		}
		else
		{
			Serial.println("SD card initialization FAILED");
		}
	}

	SD_File openFile() // returns file handle
	{
		SD_File fileSD = SD.open("data.txt", FILE_WRITES);
		return fileSD;
	}

	SD_File openNextFile() // returns file handle - opens iteratively
	{
		int it = 0;
		String tester;

		while (SD.exists(filename)) // while there already exists a file with such a name
		{
			it++;
			tester = String(it) + ".csv";
			tester.toCharArray(filename, 7);
		}

		Serial.println("Opening file: " + String(filename));

		SD_File fileSD = SD.open(filename, FILE_WRITES);
		return fileSD;
	}

	void closeFile(SD_File fileSD)
	{
		fileSD.close();
	}

	SD_File closeOpen(SD_File fileSD) // returns file handle
	{
		Serial.println("Closing and opening SD card file");
		closeFile(fileSD);
		return SD.open(filename, FILE_WRITES);
	}

	float getTimeElapsed()
	{
		return millis() - SD_time;
	}

	int writeString(SD_File fileSD, String serialized)
	{
		if (!fileSD)
		{
			Serial.println("Unable to write to SD card");
			return 0;
		}
		fileSD.println(serialized);
		return 1;
	}

	int writeHeader(SD_File fileSD)
	{
		if (!fileSD)
		{
			Serial.println("Unable to write to SD card");
			return 0;
		}

		fileSD.println("Time [ms],Lat,Lng,GPS Alt [m],Sats,Pressure [hPa],Raw Baro Alt [m],Filter Baro Alt [m],Baro Vert Vel [m/s],Baro Vert Acc [m/s^2],Baro Temp [°C],IMU Acc X [m/s^2],IMU Acc Y [m/s^2],IMU Acc Z [m/s^2],IMU Gyr X [deg/s],IMU Gyr Y [deg/s],IMU Gyr Z [deg/s],IMU Or X [deg],IMU Or Y [deg],IMU Or Z [deg]");
		return 1;
	}

	int writeSensorData(SD_File fileSD, sens_data::GpsData gpsData, sens_data::IMUData imuData, sens_data::BarometerData barData, bool verbose = false)
	{
		if (!fileSD)
		{
			Serial.println("Unable to write to SD card");
			return 0;
		}

		// GPS
		float lat = gpsData.lat;   // 1
		float lng = gpsData.lng;   // 2
		float alt = gpsData.alt;   // 3
		float sats = gpsData.sats; // 4

		// Bar
		float pressure = barData.pressure;			   // 5
		float altitude = barData.altitude;			   // 6
		float f_altitude = barData.f_altitude;		   // 7
		float f_velocity = barData.f_velocity;		   // 8
		float f_acceleration = barData.f_acceleration; // 9
		float temperature = barData.temperature;	   // 10

		// IMU
		float acc_x = imuData.acc_x; // 11
		float acc_y = imuData.acc_y; // 12
		float acc_z = imuData.acc_z; // 13
		float gyr_x = imuData.gyr_x; // 14
		float gyr_y = imuData.gyr_y; // 15
		float gyr_z = imuData.gyr_z; // 16
		float or_x = imuData.or_x;	 // 17
		float or_y = imuData.or_y;	 // 18
		float or_z = imuData.or_z;	 // 19

		if (verbose)
		{
			Serial.println(String(getTimeElapsed(), 2) + "," + String(lat, 4) + "," + String(lng, 4) + "," + String(alt, 2) + "," + String(sats) + "," + String(pressure, 2) + "," + String(altitude, 2) + "," + String(f_altitude, 2) + "," + String(f_velocity, 2) + "," + String(f_acceleration, 2) + "," + String(temperature, 1) + "," + String(acc_x, 2) + "," + String(acc_y, 2) + "," + String(acc_z, 2) + "," + String(gyr_x, 2) + "," + String(gyr_y, 2) + "," + String(gyr_z, 2) + "," + String(or_x, 2) + "," + String(or_y, 2) + "," + String(or_z, 2));
		}

		fileSD.println(String(getTimeElapsed(), 2) + "," + String(lat, 4) + "," + String(lng, 4) + "," + String(alt, 2) + "," + String(sats) + "," + String(pressure, 2) + "," + String(altitude, 2) + "," + String(f_altitude, 2) + "," + String(f_velocity, 2) + "," + String(f_acceleration, 2) + "," + String(temperature, 1) + "," + String(acc_x, 2) + "," + String(acc_y, 2) + "," + String(acc_z, 2) + "," + String(gyr_x, 2) + "," + String(gyr_y, 2) + "," + String(gyr_z, 2) + "," + String(or_x, 2) + "," + String(or_y, 2) + "," + String(or_z, 2));
		return 1;
	}

	void markPreparation(SD_File fileSD)
	{
		fileSD.println("PREPARATION STATE");
	}

	void markDrogue(SD_File fileSD)
	{
		fileSD.println("DROGUE STATE");
	}

	void markMain(SD_File fileSD)
	{
		fileSD.println("MAIN STATE");
	}

	void markDescent(SD_File fileSD)
	{
		fileSD.println("DESCENT STATE");
	}

	void readFullSD(SD_File rfile)
	{
		if (rfile)
		{
			while (rfile.available())
			{
				Serial.write(rfile.read());
			}
		}
		else
		{
			Serial.println("Cannot read SD file");
		}
		rfile.close();
	}

	void test(SD_File fileSD)
	{
		fileSD.println("Testing write SD card");
	}

};