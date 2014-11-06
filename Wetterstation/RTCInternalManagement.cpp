/*Headerfile für Pinnings

Stand:25.09.2014
Version: 1.0.0
Letzte Änderung: TR

Historie:
-Ersterstellung
*/

#include <Wire.h>
#include "Pinning.h"
#include "RTCInternalManagement.h"
#include "MCSleepMode.h"


/**
 * For documentation on input datatype see: https://github.com/adafruit/RTClib/blob/master/RTClib.h#L10
 *
 */
struct internalTime convertFromExtRTC(DateTime timeIn){
  struct internalTime timeOut;
  
  timeOut.cent  = (byte)timeIn.year()/100;	
  timeOut.year  = (byte)timeIn.year()%100;
  timeOut.month = (byte)timeIn.month();	
  timeOut.day   = (byte)timeIn.dayOfWeek();     
  timeOut.date  = (byte)timeIn.day();	
  timeOut.hour  = (byte)timeIn.hour();
  timeOut.minute= (byte)timeIn.minute();	
  timeOut.second= (byte)timeIn.second();	
  
  return timeOut;
}

/**
 * For documentation on input datatype see: https://github.com/adafruit/RTClib/blob/master/RTClib.h#L10
 *
 */
DateTime convertToExtRTC(struct internalTime timeIn){
  DateTime timeOut;
  
  timeOut = DateTime(
    (uint16_t)(((uint16_t)timeIn.cent)*100 + ((uint16_t)timeIn.year)), 
    (uint8_t)timeIn.month, 
    (uint8_t)timeIn.day,
    (uint8_t)timeIn.hour, 
    (uint8_t)timeIn.minute, 
    (uint8_t)timeIn.second
  );
  
  return timeOut;
}

/**
 * \brief DOC.
 *
 */
void setRTC(struct internalTime timeIn){
  
  RTC_SetDate(RTC, timeIn.year, timeIn.month, timeIn.date, timeIn.day);
  RTC_SetTime(RTC, timeIn.hour, timeIn.minute, timeIn.second);
}

/**
 * \brief DOC.
 *
 */
void setALARM(short minutes){
  byte byte_minutes = (byte)minutes;
  
  //handle overflow
  while(byte_minutes >= 60){  
    byte_minutes -=60;
  }
  
  if(RTC_SetTimeAlarm( RTC, 0, &byte_minutes, 0 ))
    Serial.println("Alarm was NOT set");
}
  
