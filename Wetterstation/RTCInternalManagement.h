/*Headerfile für Handling der internen RTC des MCs

Stand: 06.11.2014
Version: 1.0.0
Letzte Änderung: TR

Historie:
-Ersterstellung
*/
#ifndef _RTCINTERNLALMANAGEMENT_H_
#define _RTCINTERNLALMANAGEMENT_H_

//Includes
#include <Arduino.h>
#include <RTClib.h>

//Prototypes
void   setALARM(short minutes);
struct internalTime convertFromExtRTC(DateTime timeIn);
void   setRTC(struct internalTime timeIn);

//Structs
struct internalTime{
  byte cent;	//Jahrhundert,  3Bit
  byte year;	//...
  byte month;	//
  byte day;     //beeinflusst den Counter nicht
  byte date;	//
  byte hour;	//
  byte minute;	//
  byte second;	//
};

#endif
