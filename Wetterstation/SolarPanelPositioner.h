/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_SOLARPANELPOSITIONER_HH
#define WETTERSTATION_GUARD_SOLARPANELPOSITIONER_HH 1

#include <stdint.h>

class MotorDriver;

class SolarPanelPositioner
{

    public:

        SolarPanelPositioner(MotorDriver * driver);


        /**
         * Calibrate the compass module
         **/
        void calibrateCompass() const;


        /**
         * Set a new orientation to adjust to
         *
         * \param   orientation       the new orientation (in degrees)
         *
         **/
        void set_orientation(uint16_t orientation);


    private:

        MotorDriver * _driver;

        uint16_t _direction_setpoint;

        //bool _is_calibrated;

};

#endif
