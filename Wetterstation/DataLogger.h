/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_DATALOGGER_H
#define WETTERSTATION_GUARD_DATALOGGER_H 1

class RTC_DS1307;

class DataLogEntry
{

    public:

        int32_t pressure;

        float temperature;

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

