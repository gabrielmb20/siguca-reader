#include <SoftwareSerial.h>

SoftwareSerial rfidReader(2,3); // Digital pins 2 and 3 connect to pins 1 and 2 of the RMD6300

void setup() {  
  Serial.begin(9600);
  rfidReader.begin(9600); // the RDM6300 runs at 9600bps
  Serial.println(".....INICIANDO....."); 
}

void loop(){
  int i=0;
  if (rfidReader.available()>13){
      Serial.println("--------------------------------------------");
      Serial.println("TAG NUMBER:");
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
  



