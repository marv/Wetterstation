/* vim: set sw=4 sts=4 et ft=cpp foldmethod=syntax : */

#include "Anemometer.h"

Anemometer anemo(2, 3);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting up...");
}

void loop() {
    Serial.println("Reading anemometer measurement");

    anemometer_reading ar;
    ar = anemo.getMeasurement();

    Serial.print("CRC: ");
    Serial.println(anemo.isValid(ar));
    Serial.println();

    Serial.print("Wind direction: ");
    Serial.println(anemo.windDirectionToString(ar.fields.wind_direction));

    Serial.print("Wind speed: ");
    Serial.println(ar.fields.wind_speed);

    delay(10);
}

