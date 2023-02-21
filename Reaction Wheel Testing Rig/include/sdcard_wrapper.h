#pragma once
#include <SPIFFS.h>
#include <mySD.h>

// http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it/
// Inspiration:	default esp32 SD sample code from library ESP-FTP-Server-Lib
//https://github.com/LilyGO/TTGO-T-Beam/issues/9

// thread about LoRa and SD conflict: https://stackoverflow.com/questions/57454066/how-to-use-2-spi-devices-lora-and-sd-card-on-esp32

#define _MISO 13
#define _MOSI 12
#define _SCLK 2
#define _SD_CS 25

unsigned long SD_time = millis();
char filename[50] = "piddata.csv";

namespace sdcard
{
	int sdIterator = 0, reopenCount = 50;

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
		SD_File fileSD = SD.open(filename, FILE_WRITES);
		return fileSD;
	}

	void closeFile(SD_File fileSD)
	{
		fileSD.close();
	}

	SD_File reloadFile(SD_File fileSD) // returnds file handle
	{
		Serial.println("Reloading SD card file");
		closeFile(fileSD);
		return SD.open(filename, FILE_WRITES);
	}

	void setFilename(float P, float I, float D)
	{
		String _filename = String(P*100, 0) + "_" + String(I*100, 0) + "_" + String(D*100, 0) + ".csv";
		_filename.toCharArray(filename, 50);
		Serial.println("Set filename as: " + _filename);
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

		fileSD.println("Time [ms],Plat Ang Vel [rad/s],Wheel Speed [%],KP,KI,KD,P,I,D");
		return 1;
	}

	void writeData(SD_File fileSD, float plat_vel, float wheel_spd, float Kp, float Ki, float Kd, float P, float I, float D)
	{
		if (fileSD)
		{
			fileSD.println(String(getTimeElapsed(), 4) + "," + String(wheel_spd, 4) + "," + String(plat_vel, 4) + "," + String(Kp, 4) + "," + String(Ki, 4) + "," + String(Kd, 4) + "," + String(P, 4) + "," + String(I, 4) + "," + String(D, 4));
		}

		// automatically closes and reopens file every n readings to avoid lost data
		if (sdIterator >= reopenCount)
		{
			fileSD = reloadFile(fileSD);
			sdIterator = 0;
		}
		else
		{
			sdIterator++;
		}
	}

	void test(SD_File fileSD)
	{
		fileSD.println("Testing write SD card");
	}

};