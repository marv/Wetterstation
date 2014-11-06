/* vim: set sw=4 sts=4 et ft=cpp foldmethod=syntax : */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Adafruit_BMP085.h>
#include <SHT1x.h>
#include <Adafruit_GPS.h>

#include "Pinning.h"
#include "Anemometer.h"
#include "NTC.h"
#include "HMC6352.h"
#include "DataLogger.h"
#include "gps_module.h"

#include "SunPositionEstimator.h"
#include "MotorDriver.h"
#include "SolarPanelPositioner.h"

#include "RTCInternalManagement.h"
#include "MCSleepMode.h"

#define BATTERY_VOLTAGE_TO_VOLTS   3.3 / 1024 * 6

Anemometer anemo(PIN_ANEMOMETER_DATA, PIN_ANEMOMETER_ENABLE);
SHT1x sht1x(PIND_LUFTF_DATA, PIND_LUFTF_CLK);
NTC ntc(PINA_TEMP);

RTC_DS1307 rtc;
Adafruit_BMP085 bmp;

DataLogger logger(&rtc);

MotorDriver motor_driver(PIND_MOTOR_L, PIND_MOTOR_R, PIND_MOTOR_ENABLE);
SolarPanelPositioner positioner(&motor_driver);

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;


void setup() {
    DEBUG_SERIAL.begin(9600);
    DEBUG_SERIAL.println("Starting up...");

    Wire.begin();

    /** RTC DS1307 **/
    rtc.begin();
    if (! rtc.isrunning())
    {
        DEBUG_SERIAL.println("RTC is NOT running!");

        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    /** Clocks **/
    synchronize_clocks();


    /** SD card **/
    DEBUG_SERIAL.print("Initializing SD card...");
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(10, OUTPUT);

    // see if the card is present and can be initialized:
    if (! SD.begin(chipSelect))
    {
        DEBUG_SERIAL.println("Card failed, or not present");
        // don't do anything more:
        return;
    }

    DEBUG_SERIAL.println("card initialized.");


    /** GPS **/
    init_GPS();


    /** BMP085 **/
    if (!bmp.begin())
    {
        DEBUG_SERIAL.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
    }

    /** MotorDriver & SolarPanelPositioner **/
    motor_driver.init();
    positioner.calibrateCompass();
}

void
gather_anemometer_data(uint8_t * wind_direction, uint16_t * wind_speed)
{
    anemometer_reading ar;
    ar = anemo.getMeasurement();

    // TODO: verify CRC

    *wind_direction = ar.fields.wind_direction;
    *wind_speed = ar.fields.wind_speed;
}

void
gather_bmp085_data(float * temperature, int32_t * pressure)
{
    *temperature = bmp.readTemperature();
    *pressure = bmp.readPressure();
}

void
gather_compass_data(uint16_t * heading)
{
    HMC6352.Wake();
    *heading = HMC6352.GetHeading();
    HMC6352.Sleep();
}

void
gather_sht1x_data(float * temperature, float * humidity)
{
    *temperature = sht1x.readTemperatureC();
    *humidity = sht1x.readHumidity();
}

/**
 * Due to our setup (the anemometer rotates with the platform) the reading of
 * the wind direction is biased
 * This function subtracts the offset of 90 degrees
 *
 * wrong direction | corrected
 * ----------------------------------------
 * 0000  (N)       | 1010 (W)
 * 0001  (NNE)     | 1011 (WNW)
 *    ,            |
 *    ,            |
 *    ,            |
 * 1010  (W)       | 1000 (S)
 **/
uint16_t correct_wind_direction(uint16_t direction)
{
    return (direction + 12) % 16;
}

/**
 * Set internal RTC from external DS1307 RTC
 **/
void synchronize_clocks()
{
    char dateString[12];
    char timeString[8];

    DateTime now = rtc.now();
    sprintf(dateString, "%02d-%02d-%04d", now.day(), now.month(), now.year());

    DEBUG_SERIAL.print("DS1307 RTC date: ");
    DEBUG_SERIAL.println(dateString);

    sprintf(timeString, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    DEBUG_SERIAL.print("DS1307 RTC time: ");
    DEBUG_SERIAL.println(timeString);

    DEBUG_SERIAL.println("Setting internal RTC to date/time from ext. RTC");
    struct internalTime intTime = convertFromExtRTC(now);
    setRTC(intTime);
}

void loop()
{
    DataLogEntry log_entry;

    /* get the current time from the RTC */
    DateTime now = rtc.now();

    /* get position from GPS */
    struct gps_data gps = get_position_GPS();

    DEBUG_SERIAL.println("*** GPS Info ***");
    DEBUG_SERIAL.print("Fix: "); DEBUG_SERIAL.println(gps.fix);
    DEBUG_SERIAL.print("Lon: "); DEBUG_SERIAL.println(gps.longitude);
    DEBUG_SERIAL.print("Lat: "); DEBUG_SERIAL.println(gps.latitude);
    DEBUG_SERIAL.print("Alt: "); DEBUG_SERIAL.println(gps.altitude);
    DEBUG_SERIAL.println();

    DEBUG_SERIAL.println("Acquiring data from all sensors...");
    gather_anemometer_data(&log_entry.wind_direction, &log_entry.wind_speed);
    gather_bmp085_data(&log_entry.bmp085_temperature, &log_entry.bmp085_pressure);
    gather_compass_data(&log_entry.compass_heading);
    gather_sht1x_data(&log_entry.sht15_temperature, &log_entry.sht15_humidity);
    log_entry.ntc_temperature = ntc.readTemperature();

    /* linearize current measurement */
    log_entry.adc_solar_current = ((float)analogRead(1) + 47.31f) / 3915.48f;

    /* convert ADC value to voltage */
    log_entry.adc_battery_voltage = (float)analogRead(2) * BATTERY_VOLTAGE_TO_VOLTS;

    /* correct the anemometer reading of the wind direction */
    log_entry.wind_direction = correct_wind_direction(log_entry.wind_direction);

    logger.add_entry(log_entry);


    /** solar panel positioning **/
    uint16_t sun_position = SunPositionEstimator::get_estimate(now, &gps);
    positioner.set_orientation(sun_position);

    enterWaitMode(15, 9600);
}

