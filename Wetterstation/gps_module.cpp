/*
* gps_module.cpp
* Funktionen zur Initialisierung und dem Abrufen von Daten durch das GPS-Modul. Weiterhin sind Funktionen zum aktivieren/ deaktivieren des Moduls implementiert.
* TB 06.10.14
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
  /* Setze Baudrate des verwendeten HW-Serial und die interne Baudrate des Moduls.*/
  GPS.begin(BAUD_GPS);
  GPS_SERIAL.begin(BAUD_GPS);  
  /* Aktiviere RMC (Recommended Minimum) + GGA Datenstring, Der auch die Höheninformation enthält*/
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  /* Setze Updaterate des GPS-Modul auf 1Hz.*/
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  

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
   
  
/*Der Aufruf von read() muss in einer Schleife erfolgen. Durch die Festlegung der Aktualisierungsrate während der Initialisierung wird die Rate festgelegt, mit der neue Daten zur Verfügung stehen. Der Aufruf von read() muss solange wiederholt werden, bis ein vollständiger Datensatz vorgefunden wird. Es werden auch NMEA empfangen die Statusinformationen enthalten. Beispielsweise die verwendete Firmware Version. Die Variable data_received wird dann zu eins, wenn eine NMEA empfangen wurde, der eine gültige Position enthält. Ist kein fix vorhanden, kommt es zu einer Endlosschleife. Mit Loopcount wird der Vorgang abgebrochen, wenn kein fix vorhanden ist und die vorgegebene Zyklenzahl überschritten wird.*/
   while(data_received == 0 && loop_count < LCGETGPS ){ 
       loop_count++;    
       c = GPS.read();   
/* newNMEAreceived() wertet ein Flag aus, welches indiziert, ob ein neues NMEA empfangen wurde.*/     
      if (GPS.newNMEAreceived()) {
        /*lastNMEA() liefert den letzten empfangenen NMEA Satz zurück.*/
            nmeaSentence = GPS.lastNMEA();
        
         #if DEBUG_GPS
         Serial.println(nmeaSentence); Serial.print("\n");
         #endif
         /*parse() extrahiert die Informationen und macht diese im GPS Objekt verfügbar.*/
            GPS.parse(nmeaSentence);  
            /* Es muss sichergestellt werden, dass der Empfangene NMEA-Satz auch Positionsdaten
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
   Serial.print("Anzahl read() : "); Serial.print(loop_count); Serial.print("\n");
   Serial.print("Anzahl empfangene NMAEA: "); Serial.print(debug_nmea_count); Serial.print("\n");
#endif
   return output;
}

/* Die Funktion aktiviert das GPS-Modul. Diese Funktion ist eine interne Funktion von init_GPS(). Soll das GPS Modul aktiviert werden, so muss init_GPS() aufgerufen werden. Ansonsten werden notwendige Einstellungen nicht geladen, die bei der Deaktivierung der Spannung verloren gehen.*/
void enable_GPS(){
digitalWrite(PIND_ENABLE_GPS, HIGH); 
}
/* Deaktiviert das GPS-Modul. Nach der Deaktivierung ist das GPS-Modul spannungsfrei, verbraucht damit keine Energie mehr. Soll das Modul wieder aktiviert werden, muss init_GPS() aufgerufen werden.*/
void disable_GPS(){
digitalWrite(PIND_ENABLE_GPS, LOW); 
}
