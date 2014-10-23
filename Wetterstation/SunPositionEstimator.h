/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_SUNPOSITIONESTIMATOR_H
#define WETTERSTATION_GUARD_SUNPOSITIONESTIMATOR_H 1

#include <stdint.h>

#include <Arduino.h>
#include <RTClib.h>  // for the DateTime type

#include "gps_module.h"

class SunPositionEstimator
{

    public:

        /**
         * Estimate the angle in which the sun stands
         *
         * This method takes the current time and the position from the GPS
         * to estimate the stand of the sun on the given location
         *
         * \return The direction towards to current position of the sun (in degrees)
         **/
        static uint16_t get_estimate(const DateTime & datetime,
                                     struct gps_data * gps);


};

#endif
