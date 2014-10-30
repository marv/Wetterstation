/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <Arduino.h>

#include "HMC6352.h"
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
    /* start calibration */
    HMC6352.StartCalibration();
    delay(100);

    /* turn right for 20 seconds with speed 20 */
    _driver->turn_right(130);
    delay(20000);

    /* stop calibration and motor */
    HMC6352.StopCalibration();
    _driver->stop();
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


    int Ist_Richtung;
    int Soll_Richtung = orientation;
    int uhrzeit;
    int differenz;

    if (Soll_Richtung < 0)
    {
        Soll_Richtung = Soll_Richtung + 360;
    }


    while (1)
    {
        HMC6352.Wake();
        Ist_Richtung = HMC6352.GetHeading();
        HMC6352.Sleep();

        /*
        digitalWrite(PIND_ENDLAGE2,LOW);

        Soll_Richtung = 15 * uhrzeit - 90;
        */

        differenz = Soll_Richtung - Ist_Richtung;
        if (abs(differenz) < 10)
            break;

        Serial.println(differenz);
        motorControl(differenz);

        delay(100);
    }
}

void
SolarPanelPositioner::motorControl(int diff)
{
    if (diff < -10)
    {
        if (diff < -155)
        {
            diff = -155;
        }

        _driver->turn_right(100 - diff);
    }
    else if (diff >= 10)
    {
        if (diff > 155)
        {
            diff = 155;
        }

        _driver->turn_left(100 + diff);
    }
    else
    {
        _driver->stop();
    }
}

