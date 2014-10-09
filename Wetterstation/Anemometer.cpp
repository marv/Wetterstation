/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <Arduino.h>

#include "Anemometer.h"

static char* wind_directions[16] = {
    "N", "NNO", "NO", "ONO",
    "O", "OSO", "SO", "SSO",
    "S", "SSW", "SW", "WSW",
    "W", "WNW", "NW", "NNW"
};


Anemometer::Anemometer(int dataPin, int enablePin) :
    _dataPin(dataPin),
    _enablePin(enablePin)
{
    pinMode(dataPin, INPUT);
    pinMode(enablePin, OUTPUT);
}

anemometer_reading
Anemometer::getMeasurement() const
{
    anemometer_reading measurement;

    /* enable the onboard regulator */
    digitalWrite(_enablePin, LOW);

    /* wait for data line to become HIGH */
    while (digitalRead(_dataPin) == LOW);

    /* read 26 bit, 800bps signal */
    uint32_t value = 0;
    uint32_t mask = 1;
    for (int i = 0; i < 26; i++)
    {
        if (digitalRead(_dataPin) == LOW)
            value |= mask;

        mask <<= 1;
        delayMicroseconds(1250);
    }

    /* disable regulator */
    digitalWrite(_enablePin, HIGH);

    measurement.value = value;

    return measurement;
}

bool
Anemometer::isValid(const anemometer_reading & measurement) const
{
    uint8_t crc = (measurement.fields.wind_direction & 0x0f)
            ^ (measurement.fields.wind_speed & 0x0f00)
            ^ (measurement.fields.wind_speed & 0x00f0)
            ^ (measurement.fields.wind_speed & 0x000f);

    return (measurement.fields.crc == crc);
}

char *
Anemometer::windDirectionToString(uint8_t wind_direction) const
{
    return wind_directions[wind_direction];
}

