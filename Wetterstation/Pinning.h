/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_PINNING_H
#define WETTERSTATION_GUARD_PINNING_H 1

/**
 * Headerfile für Pinnings
 **/

/**
 * Define which serial port to use for debugging
 *
 * this makes it easier to switch between USB cable and ZigBee
 **/
#define DEBUG_SERIAL            Serial

//--------------------------------------------------------------------
//      Name                    Due-Pin     Bezeichnung
//--------------------------------------------------------------------
#define PIND_MOTOR_L            2           // Motorsteuerung L293 links
#define PIND_MOTOR_R            3           // Motorsteuerung L293 rechts
#define PIND_MOTOR_ENABLE       50          // Motor-Endlagentaster 2 (nicht verwendet)

#define PIN_ANEMOMETER_DATA     22          // Anemometer Serial Data
#define PIN_ANEMOMETER_ENABLE   23          // Anemometer Regulator Enable

#define PIND_LED                13          // Orange LED "L"

#define PINTX_GPS               16          // GPS UART TX
#define PINRX_GPS               17          // GPS UART RX

#define PINTX_XBEE              18          // XBEE UART TX
#define PINRX_XBEE              19          // XBEE UART RX

#define PINSDA_I2C              20          // RTC, Druck, Kompass I2C SDA
#define PINSCL_I2C              21          // RTC, Druck, Kompass I2C SCL

#define PIND_LUFTF_DATA         24          // Luftfeuchte-Sensor Daten
#define PIND_LUFTF_CLK          25          // Luftfeuchte-Sensor Clock

#define PIND_ENABLE_GPS			39			// TB 30.10.14 Deaktivierung des GPS_Modul 
#define PIND_CS_SDCARD          51          // Chip-Select SD-Kartenleser

#define PIND_CS_MOTOR           52          // Chip-Select Motortreiber

#define PIND_EN_AKKUMESS        53          // ENABLE Akkuspannungs-Messung

#define PINA_TEMP               0          // Analoge Temperaturspannung

#define PINSPI_DI_SDCARD        74          // DI SD-Kartenleser
#define PINSPI_DO_SDCARD        75          // DO SD-Kartenleser
#define PINSPI_CLK_SDCARD       76          // CLK SD-Kartenleser

#endif
