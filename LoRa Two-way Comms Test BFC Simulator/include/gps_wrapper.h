#pragma once
#include <TinyGPS++.h>

//* Custom definitions for working with Heltec (V3)
#define RX2 6
#define TX2 7

namespace gps
{
    TinyGPSPlus gps;
    boolean hasData = false;

    void setup(uint gpsRate = 9600)
    {
        Serial2.begin(gpsRate, SERIAL_8N1, RX2, TX2); // hardware serial 2 (RX-16, TX-17)
        Serial.println("Init GPS: " + String(gpsRate));
    }

    void readGps()
    {
        hasData = false;

        while (Serial2.available())
        {
            gps.encode(Serial2.read());
            hasData = true;
        }
    }

    void printData()
    {
        if (hasData)
        {
            Serial.print("Lat: ");
            Serial.println(gps.location.lat(), 6);
            Serial.print("Long: ");
            Serial.println(gps.location.lng(), 6);
            Serial.print("Alt: ");
            Serial.println(gps.altitude.meters(), 1);
            Serial.print("Sats: ");
            Serial.println(gps.satellites.value(), 1);
            Serial.println();
        }
    }

    void printTime()
    {
        if (hasData)
        {
            Serial.println("Time:\t" + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
        }
    }

    double lastLatitude()
    {
        return gps.location.lat();
    }

    double lastLongitude()
    {
        return gps.location.lng();
    }

    double lastAltitude()
    {
        return gps.altitude.meters();
    }

    int getSatellites()
    {
        return gps.satellites.value();
    }

    uint8_t getHour()
    {
        return gps.time.hour();
    }

    uint8_t getMinute()
    {
        return gps.time.minute();
    }

    uint8_t getSecond()
    {
        return gps.time.second();
    }

}