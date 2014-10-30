/*
* gps_module.cpp
* Functions for initialization and requesting of position data from the gps_module.
* TB 06.10.14
    
TB 15.10.14 : 
Start ohne Fix: funktioniert
Start mit fix: funktioniert
Was passiert wenn das GPS-Signal verloren geht? Nicht getestet
Was passiert wenn das GPS-Modul keine SPannung hat? Nicht getestet
*/
/**********************************************************************************
*/
#include <Adafruit_GPS.h>
#include "Pinning.h"
#include "gps_module.h"


Adafruit_GPS GPS(&GPS_SERIAL);

/*
Initialisiert das GPS Modul. Muss vor der Verwendung von get_position_GPS() einmalig aufgerufen werden.
*/
void init_GPS(){
   /*Definiere den Ausgang zur Aktivierung des GPS-Moduls als Ausgang.*/
   pinMode(PIND_ENABLE_GPS, OUTPUT);
   /*Aktiviere GPS-Modul*/
   enable_GPS();
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(BAUD_GPS);
  GPS_SERIAL.begin(BAUD_GPS);  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

}

/*
Gibt die aktuelle Position als Längen- und Breitengrad, die Höhe über dem Meeresspiegel und ob eine Satellitenverbindung besteht zurück.
Parameters:
ReturnValue: gps_data.longitude;   //Längengrad
	gps_data.latitude;         //Breitengrad
        gps_data.altitude;         //Höhe über mittlerem Meeresspiegel
	gps_data.fix;              // Satellitenverbindung vorhanden
*/
struct gps_data get_position_GPS(){
 
   short data_received = 0;   
   char c;
   struct gps_data output; 
   char * nmeaSentence = NULL;
   int loop_count=0;
    #if DEBUG_GPS
   int  debug_nmea_count=0;
   #endif
   
  
   /*Der Aufruf von read() muss in einer Schleife erfolgen. Durch die Festlegung der Aktualisierungsrate
   während der Initiliserung wird eine wird die Rate festgelegt, mit der neue Daten zur Verfügung stehen.
  Der Aufruf von read() muss solange wiederholt werden, bis ein vollständiger Datensatz vorgefunden wird.
 Es werden NMEA empfangen die Statusinformationen enthalten. Beispielsweise die verwendete Firmware Version.
Die Variable data_received wird dann zu eins wenn eine NMEA empfangen wurde, die eine gültige Position enthält.
Ist kein fix vorhanden, kann dieser Vorgang sehr lange dauern. Mit Loopcount wird der Vorgang abgebrochen,
wenn kein fix vorhanden ist und die vorgebene Zyklenzahl überschritten wird.*/
   while(data_received == 0 && loop_count < LCGETGPS ){ 
       loop_count++;    
       c = GPS.read();   
/* newNMEAreceived() wertet ein Flag aus, welches indiziert, ob ein neues NMEA empfangen wurde.*/     
      if (GPS.newNMEAreceived()) {
        /*lastNMEA() liefert den letzten empfangenen NMEA Satz zurück.*/
            nmeaSentence = GPS.lastNMEA();
        
         #if DEBUG_GPS
         DEBUG_SERIAL.println(nmeaSentence); DEBUG_SERIAL.print("\n");
         #endif
         /*parse() extrahiert die Informationen und macht diese im GPS Objekt verfügbar.*/
            GPS.parse(nmeaSentence);  
            /* Es muss sichergestellt werden, dass der Empfangene NMEA Satz, ein Satz ist der Positionsdaten
            enthält. Weiterhin muss abgefragt werden, ob die Positionsdaten gültig sind.(fix vorhanden)*/
            if(strstr(nmeaSentence,"GPGGA") != NULL && (short)GPS.fix == 1){
            data_received=1;
          }
             #if DEBUG_GPS
             debug_nmea_count++;
             #endif
     }     
     if((short)GPS.fix == 1){
          output.longitude = GPS.longitude;
          output.latitude = GPS.latitude;
          output.fix = (short)GPS.fix;
          output.altitude = GPS.altitude;
       }
      else{
             output.longitude = 0.0f;
            output.latitude = 0.0f;
             output.fix = 0;
             output.altitude = 0.0f;
       }
 }
#if DEBUG_GPS
   DEBUG_SERIAL.print("Anzahl read() : "); DEBUG_SERIAL.print(loop_count); DEBUG_SERIAL.print("\n");
   DEBUG_SERIAL.print("Anzahl empfangene NMAEA: "); DEBUG_SERIAL.print(debug_nmea_count); DEBUG_SERIAL.print("\n");
#endif
   return output;
}

/* Die Funktion aktiviert das GPS-Modul. Diese Funktion ist eine interne Funktion von init_GPS(). Soll das GPS Modul aktivert werden, somuss init_GPS() aufgerufen werden, da notwendig Einstellung ansonsten nicht geladen werden, die bei der Deaktivierung der SPannung verloren gehen.*/
void enable_GPS(){
digitalWrite(PIND_ENABLE_GPS, HIGH); 
}
/* Deaktiviert das GPS-Modul. Nach der Deaktivierung ist das GPS-Modul Spannungsfrei, verbraucht damit keine Energie mehr. Soll das Modul wieder aktiviert werden, muss init_GPS() aufgerufen werden.*/
void disable_GPS(){
digitalWrite(PIND_ENABLE_GPS, LOW); 
}
