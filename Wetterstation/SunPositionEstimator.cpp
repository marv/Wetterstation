/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "SunPositionEstimator.h"

uint16_t
SunPositionEstimator::get_estimate(const DateTime & datetime,
                                   struct gps_data * gps)
{
    uint8_t hour, minute;

    hour = datetime.hour();
    minute = datetime.minute();

    /* do we have a GPS fix? */
    if (gps->fix)
    {
        /**
         * use
         * gps.longitude & gps.latitude
         * for the current position
         **/

    }
    else
    {
        /* we don't know our exact position, make assumption
         * about position?
         */
    }

}

