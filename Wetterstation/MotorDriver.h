/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_MOTORDRIVER_H
#define WETTERSTATION_GUARD_MOTORDRIVER_H 1

#include <stdint.h>

class MotorDriver
{

    public:

        MotorDriver(uint8_t pin_left, uint8_t pin_right, uint8_t pin_enable);


        void init() const;

        void enable(bool enable) const;

        void turn_left(uint8_t speed) const;

        void turn_right(uint8_t speed) const;

        void stop() const;


    private:

        uint8_t _pin_left;

        uint8_t _pin_right;

        uint8_t _pin_enable;

};

#endif
