#include <SoftwareSerial.h>

SoftwareSerial rfidReader(2,3); // Pines digitales 2 y 3 del Arduino UNO conectados a los pines 1 (TX) y 2 (RX) del lector RFID RMD6300

void setup() {  
  Serial.begin(9600);
  rfidReader.begin(9600); // RDM6300 -> 9600bps
  Serial.println("...............INICIANDO..............."); 
}

void loop(){
  int i=0;
  if (rfidReader.available()>13){
      Serial.println("--------------------------------------------");
      Serial.println("CODIGO RFID:");
      Serial.println("--------------------------------------------");
      while(i<12){
        Serial.print(rfidReader.read());  
        Serial.print(",");     
        i++;
      }      
      Serial.print(rfidReader.read());  
      Serial.println(",3");   
      Serial.println("--------------------------------------------");
  } 
  rfidReader.flush();
  delay(4000);
  i=0;
}
