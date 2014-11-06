/*Headerfile für Pinnings

Stand:25.09.2014
Version: 1.0.0
Letzte Änderung: TR

Historie:
-Ersterstellung
*/
#ifndef _RTCINTERNLALMANAGEMENT_H_
#define _RTCINTERNLALMANAGEMENT_H_

//Prototypes
void setALARM(short minutes);

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
