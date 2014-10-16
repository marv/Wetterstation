/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef WETTERSTATION_GUARD_PINNING_H
#define WETTERSTATION_GUARD_PINNING_H 1

/**
 * Headerfile für Pinnings
 **/

//--------------------------------------------------------------------
//      Name                    Due-Pin     Bezeichnung
//--------------------------------------------------------------------
#define PIND_MOTOR_L            2           // Motorsteuerung L293 links
#define PIND_MOTOR_R            3           // Motorsteuerung L293 rechts

#define PIN_ANEMOMETER_DATA     22          // Anemometer Serial Data
#define PIN_ANEMOMETER_ENABLE   23          // Anemometer Regulator Enable

#define PIND_LED                13          // Orange LED "L"

#define PINTX_GPS               16          // GPS UART TX
#define PINRX_GPS               17          // GPS UART RX

#define PINTX_XBEE              18          // XBEE UART TX
#define PINRX_XBEE              19          // XBEE UART RX

#define PINSDA_I2C              20          // RTC, Druck, Kompass I2C SDA
#define PINSCL_I2C              21          // RTC, Druck, Kompass I2C SCL

#define PIND_LUFTF_DATA         10          // Luftfeuchte-Sensor Daten
#define PIND_LUFTF_CLK          11          // Luftfeuchte-Sensor Clock

#define PIND_ENDLAGE1           49          // Motor-Endlagentaster 1
#define PIND_ENDLAGE2           50          // Motor-Endlagentaster 2 (nicht verwendet)

#define PIND_CS_SDCARD          51          // Chip-Select SD-Kartenleser

#define PIND_CS_MOTOR           52          // Chip-Select Motortreiber

#define PIND_EN_AKKUMESS        53          // ENABLE Akkuspannungs-Messung

#define PINA_TEMP               54          // Analoge Temperaturspannung

#define PINSPI_DI_SDCARD        74          // DI SD-Kartenleser
#define PINSPI_DO_SDCARD        75          // DO SD-Kartenleser
#define PINSPI_CLK_SDCARD       76          // CLK SD-Kartenleser

#endif
