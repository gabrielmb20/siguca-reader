// ------------------------------------------------------------------------------------------------------------------------------------
// LIBRERÍAS
// ------------------------------------------------------------------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h> 
#include "Nextion.h"


// ------------------------------------------------------------------------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------------------------------------------------------------------------
// Utilizando múltiples puertos "Software Serial", sólo uno puede recibir información a la vez.
SoftwareSerial RFID(2, 3); // RFID TX al pin 4 y RX al pin 5 del Arduino 
SoftwareSerial nextion(4, 5);// Nextion TX al pin 2 y RX al pin 3 del Arduino
SoftwareSerial ESP(6, 7); // ESP TX al pin 6 y RX al pin 7 del Arduino
Nextion myNextion(nextion, 9600); // Crear un objeto Nextion llamado myNextion usando un puerto serial a 9600bps
int data1 = 0;
int ok = -1;
String boton;
String WSSID = "siguca-pi";
String WPASS = "sigucareader-pi";
//String WSSID = "Home";
//String WPASS = "darkside";
//String WSSID = "gcs-soporte";
//String WPASS = "Ufawuow1otho";
bool r;
int codRFID;
// ------------------------------------------------------------------------------------------------------------------------------------
// CODIGOS VALIDOS
// ------------------------------------------------------------------------------------------------------------------------------------
// Llavero: 0004199764
int tag1[14] = {2,53,57,48,48,52,48,49,53,53,52,53,56,3};
// Tarjeta: 0006488185
int tag2[14] = {2,50,48,48,48,54,51,48,48,55,57,51,65,3};
// Usado para comparaciones
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 


// ------------------------------------------------------------------------------------------------------------------------------------
// SETUP
// -----------------------------------------------------------   -------------------------------
void setup(){
  Serial.begin(9600);   
  myNextion.init(); // Inicializar el objeto serial para la pantalla Nextion
  // Por defecto, el último obtejo serial inicializado queda escuchando
  RFID.begin(9600); // Inicializar el objeto serial para la lector RFID  
}


// ------------------------------------------------------------------------------------------------------------------------------------
// COMPARAR CODIGO
// ------------------------------------------------------------------------------------------------------------------------------------
boolean compararCodigo(int aa[14], int bb[14]){
  boolean ff = false;
  int fg = 0;
  for (int cc = 0 ; cc < 14 ; cc++)  {
    if (aa[cc] == bb[cc])    {
      fg++;
    }
  }
  if (fg == 14)  {
    ff = true;
  }
  return ff;
}

// ------------------------------------------------------------------------------------------------------------------------------------
// VALIDAR CODIGO
// ------------------------------------------------------------------------------------------------------------------------------------ 
void validarCodigo(){
  // Variable para decidir si el codigo es valido o no
  // 1 = lectura de bit acertada
  // 0 = lectura de bit no acertada
  // -1 = no lectura
  ok = 0; 
  if (compararCodigo(newtag, tag1) == true){
    codRFID=1;
    ok++;
  }
  if (compararCodigo(newtag, tag2) == true){
    ok++;
    codRFID=2;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------
// LEER CODIGO
// ------------------------------------------------------------------------------------------------------------------------------------
void leerCodigo(){
  myNextion.sendCommand("page Inicio");  
  RFID.listen();
  delay(2000);  
  ok = -1; 
  if (RFID.available() > 0){
    // Lectura del codigo RFID, se necesitan algunos segundos para permitir que la informacion ingrese en el bufer serial.
    delay(150);  
    // Lectura de los 14 bits del codigo
    for (int z = 0 ; z < 14 ; z++){
      data1 = RFID.read();
      newtag[z] = data1;
    }
    // Evita multiples lecturas
    RFID.flush();  
    // Revision del codigo para decidir si es valido
    validarCodigo();
  } 
  
// ------------------------------------------------------------------------------------------------------------------------------------
// PROCESAR CODIGO
// ------------------------------------------------------------------------------------------------------------------------------------
  // Codigo Valido
  if (ok > 0){
    Serial.println("Acceso Valido");    
    digitalWrite(13, HIGH);
//    delay(2000);
//    digitalWrite(13, LOW); 
    seleccionarBoton();
    ok = -1;
  }
  // Codigo Invalido
  else if (ok == 0){
    Serial.println("Acceso Invalido");
    digitalWrite(12, HIGH);
    delay(1000);
    digitalWrite(12, LOW); 
    ok = -1;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------
// SELECCIONAR BOTON
// ------------------------------------------------------------------------------------------------------------------------------------
void seleccionarBoton(){
   myNextion.sendCommand("page Escritorio");  

   
   myNextion.sendCommand("p0.pic=7");


   
   nextion.listen();
   boton="";
   while(boton == ""){
    boton = myNextion.listen(); //check for message
   }
   Serial.println(boton);
   String url;
   String marca;
   int getRequestLength;
   myNextion.sendCommand("page MarcaRealizada");  
   inicializarESP();
   ESP.println("AT+CIPSTART=\"TCP\",\"10.10.10.10\",3000");
   delay(1000);

 
   //-------------------------------------------------------//
   if (boton == "65 1 1 0 ffff ffff ffff") {              
        Serial.println("ENTRADA");
        marca = "1";
   }
   //-------------------------------------------------------//
   if (boton == "65 1 2 0 ffff ffff ffff") {
        Serial.println("INICIO RECESO");
        marca = "2";

   }
   //-------------------------------------------------------//
   if (boton == "65 1 3 0 ffff ffff ffff") {
        Serial.println("INICIO ALMUERZO");
        marca = "4";
 
   }
   //-------------------------------------------------------//
   if (boton == "65 1 4 0 ffff ffff ffff") {
        Serial.println("SALIDA");
        marca = "6";
 
   }
   //-------------------------------------------------------//
   if (boton == "65 1 5 0 ffff ffff ffff") {
        Serial.println("FIN RECESO");
        marca = "3";

   }
   //-------------------------------------------------------//
   if (boton == "65 1 6 0 ffff ffff ffff") {
        Serial.println("FIN ALMUERZO");
        marca = "5";

   }
   //-------------------------------------------------------//
   url = "GET /rfidReader?pwd1=ooKa6ieC&pwd2=of2Oobai&codTarjeta=111&tipoMarca=" + marca + " HTTP/1.1\r\nHost: 10.10.10.10\r\n";
   getRequestLength = url.length() + 2; // add 2 because \r\n will be appended by SoftwareSerial.println().
   ESP.println("AT+CIPSEND=" + String(getRequestLength));
   delay(1000);
   espEnviarComando( url , "+IPD" , 3000 );  
//   ESP.println("AT+RST");////////////
   digitalWrite(13, LOW); 
}


// ------------------------------------------------------------------------------------------------------------------------------------
// inicializarESP
// ------------------------------------------------------------------------------------------------------------------------------------
void inicializarESP() {
  ESP.begin(9600); // default baud rate for ESP8266
  Serial.println("\n... VERIFICANDO CONEXION DEL ESP...\n");
//  ESP.println("AT+RST");
//  delay(2000);
//  ESP.println("AT+CWMODE=3");
//  delay(1000);
//  ESP.println("AT+CWJAP=\""+WSSID+"\",\""+WPASS+"\"");
//  delay(9000);
  Serial.println("\n\nESP CONECTADO\n");
}


// ------------------------------------------------------------------------------------------------------------------------------------
// espEnviarComando
// ------------------------------------------------------------------------------------------------------------------------------------
bool espEnviarComando(String cmd, String goodResponse, unsigned long timeout) {
  Serial.println("espEnviarComando( " + cmd + " , " + goodResponse + " , " + String(timeout) + " )" );
  ESP.println(cmd);
  unsigned long tnow = millis();
  unsigned long tstart = millis();
  unsigned long execTime = 0;
  String response = "";
  char c;
  while( true ) {
    if( tnow > tstart + timeout ) {
      Serial.println("ERROR -> TIMEOUT " );
      return false;
    }
    c = ESP.read();
    if( c >= 0 ) {
      response += String(c);
      if( response.indexOf(goodResponse) >= 0 ) {
        execTime = ( millis() - tstart );
        Serial.println("Comando Enviado Exitosamente - Tiempo Respuesta: " + String(execTime) + "ms");
        Serial.println("Respuesta: " + response);
//        Serial.println(response);
        while(ESP.available() > 0) {
          Serial.write(ESP.read());
        }
        return true;
      }
    }
    tnow = millis();
  }
}


// ------------------------------------------------------------------------------------------------------------------------------------
// espGET
// ------------------------------------------------------------------------------------------------------------------------------------

void espGET(String url){ 
  ESP.println("AT+CIPSTART=\"TCP\",\"10.10.10.10\",3000");
  delay(2000);
  int getRequestLength = url.length() + 2; // add 2 because \r\n will be appended by SoftwareSerial.println().
  ESP.println("AT+CIPSEND=" + String(getRequestLength));
  delay(2000);
  espEnviarComando( url , "+IPD" , 5000 );
  
}

  
// ------------------------------------------------------------------------------------------------------------------------------------
// LOOP
// ------------------------------------------------------------------------------------------------------------------------------------
void loop(){
   leerCodigo();
 
  

//  inicializarESP();
//  String request = "GET /rfidReader?pwd1=ooKa6ieC&pwd2=of2Oobai&codTarjeta=111&tipoMarca=1 HTTP/1.1\r\nHost: 10.10.10.10\r\n";
//  espGET(request);

}
