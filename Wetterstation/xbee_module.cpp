/*
* xbee_module.cpp
* FUnktionen zum senden und empfangen von Strings über Xbee. Die Xbees müssen im API-Mode laufen.
* TB 30.10.14
 

*/
/**********************************************************************************
*/
#include <Arduino.h>
#include "Pinning.h"
#include "xbee_module.h"
#include "XBee.h"

/**/
XBee xbee = XBee();
//short handshake_flag_xbee = 1;

/*Initialisert das XBee Modul. Muss vor der Verwendung einmalig aufgerufen werden.*/
void init_xbee(){

	SERIAL_XBEE_SENDEN.begin(BAUD_XBEE);//Verbindung Xbee <-> Arduino
	xbee.setSerial(SERIAL_XBEE_SENDEN);
	#if DEBUG_XBEE
	Serial.print("\nEnde init_XBEE()!");
	#endif
}

/*Sendet einen Datenstring über das Xbee. DIe Länge darf maximal MAX_STR_XBEE-1 betragen betragen. Nach dem senden erfolgt ein Handshake, hierber blockiert die Ausführung bis zu TIME_STATUS_RESPONES_XBEE Millisekunden.
Parameter: char str [] : Der zu übertragene String.
*/
void send_to_xbee(char str []){
#if DEBUG_XBEE
      Serial.print("Enter send_to_xbee()!\n");				
#endif
/*Erstelle einen Sende Request*/    
TxStatusResponse txStatus = TxStatusResponse();
#if DEBUG_XBEE
      Serial.print("SizeOFpayload: "); Serial.println(strlen(str));			
      Serial.print("PayloadText: "); Serial.println( str);
#endif
Tx16Request tx = Tx16Request(ADRESS_XBEE_REECEIVER,(unsigned char *) str, strlen(str));		   
xbee.send(tx);  
#if ACTIVATE_HANDSHAKE_XBEE
    /*Warten auf Status Response des Empfängers*/
    if (xbee.readPacket(TIME_STATUS_RESPONES_XBEE)) {
      
        /*Status Response erhalten*/                
        if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
           xbee.getResponse().getZBTxStatusResponse(txStatus);
                
          /*Übertragungsstauts überprüfen*/
           if (txStatus.getStatus() == SUCCESS) {
		  // handshake_flag_xbee = 1;
#if ACTIVATE_STATUS_RESPONSE_XBEE 
                Serial.print("Senden erfolgreich!\n");				
#endif
           } else {
               /*Empfänger Xbee hat das Paket nicht erhalten.*/
#if ACTIVATE_STATUS_RESPONSE_XBEE 
                Serial.print("Senden fehlgeschlagen!\n");
#endif
           }
        }      
    } else {
     /*Sender hat einen Fehler*/
#if ACTIVATE_STATUS_RESPONSE_XBEE 
                Serial.print("Senden fehlgeschlagen!\n");
#endif
    }
#endif
}

/*Liest den vollständigen aktuell verfügbaren Datenstring vom Xbee und liefert diesen zurück. Die aufrufende Funktion sollte den Rückgabewert auswerten, ob ein Datenstring gelesen wurde!
char out []: der gelesene String
return : 1 wenn etwas gelesen wurde, ansonsten 0.
*/
short read_from_xbee(char out []){
#if DEBUG_XBEE
      Serial.print("Enter read_from_xbee()!\n");				
#endif
short k=0;
short rxFlag=0;
Rx16Response rx16 = Rx16Response();
char data[MAX_STR_XBEE]="Q";
  //empfangen
   xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      /*Es wurde etwas empfangen.*/
      
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
        /*Receive Paket erhalten.*/
        xbee.getResponse().getRx16Response(rx16);
        for(k=0;k<rx16.getDataLength();k++){
          /*Extrahiere die Zeichen aus dem Datenstring.*/
        data[k] = rx16.getData(k);       
       }
       rxFlag=1;
#if DEBUX_XBEE
	   Serial.println(data);	   
#endif
    }
}
	strcpy(out,data);
        return rxFlag;	
}
