/* vim: set sw=4 sts=4 et ft=cpp foldmethod=syntax : */

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_BMP085.h>
#include <HMC6352.h>
#include <SHT1x.h>

#include "Pinning.h"
#include "Anemometer.h"

Anemometer anemo(PIN_ANEMOMETER_DATA, PIN_ANEMOMETER_ENABLE);
SHT1x sht1x(PIND_LUFTF_DATA, PIND_LUFTF_CLK);

RTC_DS1307 rtc;
Adafruit_BMP085 bmp;

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


    /** BMP085 **/
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
    }
}

void
gather_anemometer_data()
{
    anemometer_reading ar;
    ar = anemo.getMeasurement();

    /*
    // TODO: verify CRC
    Serial.print("CRC: ");
    Serial.println(anemo.isValid(ar));
    Serial.println();
    */

    Serial.print("  Wind direction: ");
    Serial.println(anemo.windDirectionToString(ar.fields.wind_direction));

    Serial.print("  Wind speed: ");
    Serial.println(ar.fields.wind_speed);
}

void
gather_bmp085_data()
{
    Serial.print("  Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("  Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("  Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("  Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

    // you can get a more precise measurement of altitude
    // if you know the current sea level pressure which will
    // vary with weather and such. If it is 1015 millibars
    // that is equal to 101500 Pascals.
    Serial.print("  Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
}

void
gather_compass_data()
{
    HMC6352.Wake();
    int heading = HMC6352.GetHeading();
    Serial.print("  Heading: ");
    Serial.println(heading);
    HMC6352.Sleep();
}

void
gather_sht1x_data()
{
    float temp_c;
    float humidity;

    // Read values from the sensor
    temp_c = sht1x.readTemperatureC();
    humidity = sht1x.readHumidity();

    // Print the values to the serial port
    Serial.print("  Temperature: ");
    Serial.print(temp_c, DEC);
    Serial.print(" *C, ");
    Serial.print(humidity);
    Serial.println("%");
}

void loop() {
    Serial.println("Acquiring data from all sensors...");

    Serial.println(" * Anemometer:");
    gather_anemometer_data();

    Serial.println(" * BMP085:");
    gather_bmp085_data();

    Serial.println("* HMC6352 compass");
    gather_compass_data();

    Serial.println(" * SHT1x:");
    gather_sht1x_data();

    delay(10);
}

