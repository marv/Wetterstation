/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_ANEMOMETER_HH
#define WETTERSTATION_GUARD_ANEMOMETER_HH 1

#include <stdint.h>

union anemometer_reading
{
      uint32_t value;

      struct
      {
          unsigned int start_marker   :  5;
          unsigned int wind_direction :  4;
          unsigned int wind_speed     : 12;
          unsigned int crc            :  4;
          unsigned int padding        :  7;
      } fields;
};

class Anemometer
{
    public:

        Anemometer(int dataPin, int enablePin);

        /**
         * Get measured wind speed and direction
         *
         * ATTENTION: There is no way to request a measurement
         * from the anemometer. When it's powered up it just will
         * broadcast the current measurement in a fixed interval.
         * Therefore this function will poll the data line in order
         * to detect a message transmission and then read it.
         **/
        anemometer_reading getMeasurement() const;

        /**
         * Check if measurement data is valid
         *
         * The reading contains a CRC checksum, which will
         * be used to check the validity of the reading
         **/
        bool isValid(const anemometer_reading & measurement) const;


        /**
         * Returns the corresponding human readable wind direction
         * for a raw wind direction code
         *
         * For example 
         *    0   =>  "N"    (Nord)
         *    1   =>  "NNO"  (Nord-Nord-Ost)
         *    [..]
         *
         **/
        char *windDirectionToString(uint8_t wind_direction) const;

    private:

        int _dataPin;

        int _enablePin;

};

#endif
