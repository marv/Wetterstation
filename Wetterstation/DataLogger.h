/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_DATALOGGER_H
#define WETTERSTATION_GUARD_DATALOGGER_H 1

class RTC_DS1307;

class DataLogEntry
{

    public:

        int32_t  bmp085_pressure;
        float    bmp085_temperature;

        float    sht15_temperature;
        float    sht15_humidity;

        uint8_t  wind_direction;
        uint16_t wind_speed;

        uint16_t compass_heading;

};

class DataLogger
{

    public:

        DataLogger(RTC_DS1307 * rtc);


        void add_entry(const DataLogEntry & entry);


    private:

        RTC_DS1307 * _rtc;

};

#endif

