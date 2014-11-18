/*
* xbee_module.h
* Headerfile für das xbee-Module
* TB 30.10.14
*/

#ifndef XBEE_MODULE_H
#define XBEE_MODULE_H

/**********************************************************************************
*Defines
*/
/*Aktiviert Debug features.  1 oder 0*/
#define DEBUG_XBEE 0
/*Aktiviert AUsgabe der Sendebestätigung*/
#define ACTIVATE_STATUS_RESPONSE_XBEE 0
/* Transmission speed XBEE-Modul <-> Arduino-Board. 9600 Baud */
#define BAUD_XBEE 9600
/* Zu verwendener Hardware UART. Serial 2 entspricht Rx2Tx2*/
#define SERIAL_XBEE_SENDEN Serial2

/* Addresse Xbee Empfänger. Kann mit der Software XCTU ermittelt werden*/
#define ADRESS_XBEE_REECEIVER 0x2067

/*Max Länge des empfangen und gesendeten Strings beträgt MAX_STR_XBEE-1.*/
#define MAX_STR_XBEE 100

/*Wartezeit für Statusresponse nach dem senden.*/
#define TIME_STATUS_RESPONES_XBEE 5000

/*Aktiviert, bzw deaktiviert die Handshake Funktion des xbee. Sollte stets aktiv sein.*/
#define ACTIVATE_HANDSHAKE_XBEE 1

/*Anzahl Handshake Versuche bis bei wiederholten fehlschlagen eine neue Nachricht gesendet werden darf.*/
//#define HANDSHAKE_ATTEMPTS_XBEE 5
/**********************************************************************************
*Prototypes
*/
void init_xbee();
void send_to_xbee(char inp []);
short read_from_xbee(char out []);

#endif
