/*Skript für Handling der internen RTCs

Stand: 06.11.2014
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
 * \brief Converts a struct of type DateTime to an internal time struct.
 * For documentation on input datatype see: https://github.com/adafruit/RTClib/blob/master/RTClib.h#L10
 *
 * \param timeIn A struct of type DateTime.
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
 * \brief Converts an internal time struct to a struct of type DateTime
 * For documentation on output datatype see: https://github.com/adafruit/RTClib/blob/master/RTClib.h#L10
 *
 * \param timeIn A struct of the internal time type
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
 * \brief Sets the internal RTC based on the parameter.
 *
 * \param timeIn the struct used to set the internal RTC
 */
void setRTC(struct internalTime timeIn){
  
  RTC_SetDate(RTC, timeIn.year, timeIn.month, timeIn.date, timeIn.day);
  RTC_SetTime(RTC, timeIn.hour, timeIn.minute, timeIn.second);
}

/**
 * \brief Sets the alarm of the internal RTC.
 * Before setting the alarm to the value of the parameter the parameter is checked if 
 * it exceeds the maximum value of 59 and reduced by 60 until it satisfies the expectation.
 * Apart from that a Error message will be sent via Serial if the alarm could not be set.
 *
 * \param minutes The minute which the alarm will be set to.
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
  
