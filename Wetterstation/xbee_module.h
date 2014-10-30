/*
* xbee_module.h
* Headerfile for the xbee-Module
* TB 30.10.14
*/

#ifndef XBEE_MODULE_H
#define XBEE_MODULE_H

/**********************************************************************************
*Defines
*/
/*Aktiviert Debug features.  1 oder 0*/
#define DEBUG_XBEE 0
/* Transmission speed XBEE-Modul <-> Arduino-Board. 9600 Baud */
#define BAUD_XBEE 9600
/* Zu verwendener Hardware UART. Serial 2 entspricht Rx2Tx2*/
#define SERIAL_XBEE Serial2
/*Max Länge des empfangen und gesendeten String*/
#define MAX_READ_STR_XBEE 100
/**********************************************************************************
*Prototypes
*/
void init_xbee();
void send_to_xbee(char * inp);
char * read_from_xbee();

#endif
