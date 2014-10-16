/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_NTC_H
#define WETTERSTATION_GUARD_NTC_H 1

class NTC
{

    public:

        NTC(int pinNumber);


        float readTemperature() const;

    private:

        int _pinNumber;

};

#endif
