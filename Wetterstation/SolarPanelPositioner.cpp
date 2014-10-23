/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include "MotorDriver.h"

#include "SolarPanelPositioner.h"

SolarPanelPositioner::SolarPanelPositioner(MotorDriver * driver) :
    _driver(driver),
    _direction_setpoint(0)
{
}

void
SolarPanelPositioner::calibrateCompass() const
{
    // Insert code to calibrate the compass
}

void
SolarPanelPositioner::set_orientation(uint16_t orientation)
{
    /**
     * use
     *  _driver->turn_left()
     *  _driver->turn_right()
     *  _driver->stop()
     * to make the motor move accordingly
     *
     * read compass using
     *
     * HMC6352.Wake();
     * heading = HMC6352.GetHeading();
     * HMC6352.Sleep();
     *
     * in order to get the current orientation
     **/
}

