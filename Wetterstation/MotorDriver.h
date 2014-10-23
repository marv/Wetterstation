/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_MOTORDRIVER_H
#define WETTERSTATION_GUARD_MOTORDRIVER_H 1

#include <stdint.h>

class MotorDriver
{

    public:

        MotorDriver(uint8_t pin_left, uint8_t pin_right);


        void turn_left() const;

        void turn_right() const;

        void stop() const;

        /** method to control velocity? **/

    private:

        uint8_t _pin_left;

        uint8_t _pin_right;

};

#endif
