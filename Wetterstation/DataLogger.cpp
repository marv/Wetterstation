/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <stdlib.h>
#include <avr/dtostrf.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

#include "DataLogger.h"

DataLogger::DataLogger(RTC_DS1307 * rtc) :
    _rtc(rtc)
{
}

void
DataLogger::add_entry(const DataLogEntry & entry)
{
    char dateString[10];

    Serial.println("Adding new log entry...");

    Serial.println("Querying RTC to get date");
    DateTime now = _rtc->now();

    sprintf(dateString, "%04d%02d%02d", now.year(), now.month(), now.day());
    Serial.print("DS1307 RTC date: ");
    Serial.println(dateString);

    char filename[15];
    sprintf(filename, "%s.csv", dateString);
    filename[14] = '\0';

    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile)
    {
        char log_line[128];
        char timeString[9];

        sprintf(timeString, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
        Serial.print("DS1307 RTC time: ");
        Serial.println(timeString);

        char bmp085_temperatureString[5];
        char sht15_temperatureString[5];
        char sht15_humidityString[5];
        char ntc_temperatureString[5];
        dtostrf(entry.bmp085_temperature, 5, 2, bmp085_temperatureString);
        dtostrf(entry.sht15_temperature, 5, 2, sht15_temperatureString);
        dtostrf(entry.sht15_humidity, 5, 2, sht15_humidityString);
        dtostrf(entry.ntc_temperature, 5, 2, ntc_temperatureString);

        sprintf(log_line, "%s;%d;%d;%d;%d;%s;%s;%s;%s\n",
                timeString,
                entry.compass_heading,
                entry.wind_direction,
                entry.wind_speed,
                entry.bmp085_pressure,
                bmp085_temperatureString,
                sht15_temperatureString,
                sht15_humidityString,
                ntc_temperatureString);

        dataFile.println(log_line);
        dataFile.close();

        // print to the serial port too:
        Serial.println(log_line);
    }
    else
    {
        Serial.println("error opening datalog.txt");
    }

}

