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

Anemometer anemo(PIN_ANEMOMETER_DATA, PIN_ANEMOMETER_ENABLE);
SHT1x sht1x(PIND_LUFTF_DATA, PIND_LUFTF_CLK);
NTC ntc(PINA_TEMP);

RTC_DS1307 rtc;
Adafruit_BMP085 bmp;

DataLogger logger(&rtc);

MotorDriver motor_driver(PIND_MOTOR_L, PIND_MOTOR_R);
SolarPanelPositioner positioner(&motor_driver);

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting up...");

    Wire.begin();

    /** RTC DS1307 **/
    rtc.begin();
    if (! rtc.isrunning())
    {
        Serial.println("RTC is NOT running!");

        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    char dateString[12];
    char timeString[8];

    DateTime now = rtc.now();
    sprintf(dateString, "%02d-%02d-%04d", now.day(), now.month(), now.year());

    Serial.print("DS1307 RTC date: ");
    Serial.println(dateString);

    sprintf(timeString, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    Serial.print("DS1307 RTC time: ");
    Serial.println(timeString);


    /** SD card **/
    Serial.print("Initializing SD card...");
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(10, OUTPUT);

    // see if the card is present and can be initialized:
    if (! SD.begin(chipSelect))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
    }

    Serial.println("card initialized.");


    /** GPS **/
    init_GPS();


    /** BMP085 **/
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
    }

    /** SolarPanelPositioner **/
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

void loop()
{
    DataLogEntry log_entry;

    /* get the current time from the RTC */
    DateTime now = rtc.now();

    /* get position from GPS */
    struct gps_data gps = get_position_GPS();


    Serial.println("Acquiring data from all sensors...");
    gather_anemometer_data(&log_entry.wind_direction, &log_entry.wind_speed);
    gather_bmp085_data(&log_entry.bmp085_temperature, &log_entry.bmp085_pressure);
    gather_compass_data(&log_entry.compass_heading);
    gather_sht1x_data(&log_entry.sht15_temperature, &log_entry.sht15_humidity);
    log_entry.ntc_temperature = ntc.readTemperature();

    logger.add_entry(log_entry);


    /** solar panel positioning **/
    uint16_t sun_position = SunPositionEstimator::get_estimate(now, &gps);
    positioner.set_orientation(sun_position);


    delay(20000);
}

