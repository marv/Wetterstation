/*
* gps_module.h
* Headerfile for the GPS-Modul
* TB 06.10.14
*/
/**********************************************************************************
* Custom data types
*/
#ifndef GPS_MODULE_H
#define GPS_MODULE_H

struct gps_data{
  	float longitude;
	float latitude;
	short fix;
  };
/**********************************************************************************
* Prototypes
**/
struct gps_data get_position_GPS();
void init_GPS();

/**********************************************************************************
*Defines
*/
/*Aktiviert Debug features wie Echo-Modus. 1 oder 0*/
#define DEBUG_GPS 0
/* Transmission speed GPS-Modul <-> Arduino-Board. 9600 Baud */
#define BAUD_GPS 9600
/* Zu verwendener Hardware UART. Serial 1 entspricht Rx1Tx1*/
#define GPS_SERIAL Serial1
/* Loopcount GetPosition : Tabelle für eine Aktualisierungsrate von 1Hz
    Wert          Blockierzeit in s
    10000000      19
     5000000      9
     1000000      2
 Sollte nicht kleiner als 1000000 gewählt werden.    
*/
#define LCGETGPS 1000000                            

#endif
