/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t pin_left, uint8_t pin_right) :
    _pin_left(pin_left),
    _pin_right(pin_right)
{
}

void
MotorDriver::turn_left() const
{
    /* do magic to make motor turn left */
}

void
MotorDriver::turn_right() const
{
    /* do magic to make motor turn left */
}

void
MotorDriver::stop() const
{
    /* do magic to make motor stop */
}
