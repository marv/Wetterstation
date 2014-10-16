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

    sprintf(dateString, "%02d-%02d-%04d", now.day(), now.month(), now.year());
    Serial.print("DS1307 RTC date: ");
    Serial.println(dateString);


    char filename[15];
    sprintf(filename, "%s.log", dateString);
    filename[14] = '\0';

    File dataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {

        char log_line[128];

        char temperatureString[5];
        dtostrf(entry.temperature, 5, 2, temperatureString);

        sprintf(log_line, "%d,%s\n", entry.pressure, temperatureString);

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

