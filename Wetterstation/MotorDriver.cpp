/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <Arduino.h>

#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t pin_left, uint8_t pin_right, uint8_t pin_enable) :
    _pin_left(pin_left),
    _pin_right(pin_right),
    _pin_enable(pin_enable)
{
}

void
MotorDriver::init() const
{
    pinMode(_pin_left, OUTPUT);
    pinMode(_pin_right, OUTPUT);
    pinMode(_pin_enable, OUTPUT);
}

void
MotorDriver::turn_left(uint8_t speed) const
{
    digitalWrite(_pin_enable, HIGH);
    analogWrite(_pin_left, speed);
    analogWrite(_pin_right, 0);
    digitalWrite(_pin_enable, LOW);
}

void
MotorDriver::turn_right(uint8_t speed) const
{
    digitalWrite(_pin_enable, HIGH);
    analogWrite(_pin_left, 0);
    analogWrite(_pin_right, speed);
    digitalWrite(_pin_enable, LOW);
}

void
MotorDriver::stop() const
{
    analogWrite(_pin_left, 0);
    analogWrite(_pin_right, 0);
    digitalWrite(_pin_enable, LOW);
}
