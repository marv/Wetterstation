/*
* xbee_module.cpp
* Functions for initialization and requesting of position data from the gps_module.
* TB 30.10.14
 
*/
/**********************************************************************************
*/
#include <Arduino.h>
#include "Pinning.h"
#include "xbee_module.h"

void init_xbee(){

	SERIAL_XBEE.begin(9600);//Verbindung Xbee <-> Arduino
	#if DEBUG_XBEE
	Serial.print("\nEnde init_XBEE()!");
	#endif
}

/*Sendet einen Datenstring an das Xbee*/
void send_to_xbee(char * str){
SERIAL_XBEE.print(str);
}

/*Liest den vollst�ndigen aktuell verf�gbaren Datenstring vom Xbee und liefert diesen zur�ck.*/
char * read_from_xbee(){
  
char out [MAX_READ_STR_XBEE];
char temp;
short i=0;
	while(1){
		if (SERIAL_XBEE.available() > 0) {
			temp = SERIAL_XBEE.read();
			if (temp != -1){
				out[i] = temp;
				i++;	  
			}
		}
		else{
			out[i] = 0x00;
			return out;
		}
	}
}
