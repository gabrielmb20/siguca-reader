// ------------------------------------------------------------------------------------------
// Librerías
// ------------------------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h> 
#include "Nextion.h"

// Utilizando múltiples puertos "Software Serial", sólo uno puede recibir información a la vez. 

SoftwareSerial nextion(2, 3);// Nextion TX al pin 2 y RX al pin 3 del Arduino UNO
SoftwareSerial RFID(4, 5); // RFID TX al pin 4 y RX al pin 5 del Arduino UNO
Nextion myNextion(nextion, 9600); // Crear un objeto Nextion llamado myNextion usando un puerto serial a 9600bps
int data1 = 0;
int ok = -1;
int yes = 13;
int no = 12;
String boton;

// ------------------------------------------------------------------------------------------
// CODIGOS VALIDOS
// ------------------------------------------------------------------------------------------
// Llavero: 0004199764
int tag1[14] = {2,53,57,48,48,52,48,49,53,53,52,53,56,3};
// Tarjeta: 0006488185
int tag2[14] = {2,50,48,48,48,54,51,48,48,55,57,51,65,3};
// Usado para comparaciones
int newtag[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 


// ------------------------------------------------------------------------------------------
// SETUP
// -----------------------------------------------------------   -------------------------------
void setup(){
  Serial.begin(9600);  // start serial to PC 
  myNextion.init();
  RFID.begin(9600);    // start serial to RFID reader
// By default, the last intialized port is listening.
  
  
  
//  pinMode(yes, OUTPUT); // for status LEDs
//  pinMode(no, OUTPUT); 
  
}


// ------------------------------------------------------------------------------------------
// COMPARAR CODIGO
// ------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------
// VALIDAR CODIGO
// ------------------------------------------------------------------------------------------ 
void validarCodigo(){
  // Variable para decidir si el codigo es valido o no
  // 1 = lectura de bit acertada
  // 0 = lectura de bit no acertada
  // -1 = no lectura
  ok = 0; 
  if (compararCodigo(newtag, tag1) == true){
    ok++;
  }
  if (compararCodigo(newtag, tag2) == true){
    ok++;
  }
}

// ------------------------------------------------------------------------------------------
// LEER CODIGO
// ------------------------------------------------------------------------------------------
void leerCodigo(){
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
  
// ------------------------------------------------------------------------------------------
// PROCESAR CODIGO
// ------------------------------------------------------------------------------------------
  // Codigo Valido
  if (ok > 0){
    Serial.println("Acceso Valido");    
    seleccionarBoton();
    digitalWrite(yes, HIGH);
    delay(1000);
    digitalWrite(yes, LOW); 
    ok = -1;
  }
  // Codigo Invalido
  else if (ok == 0){
    Serial.println("Acceso Invalido");
    digitalWrite(no, HIGH);
    delay(1000);
    digitalWrite(no, LOW); 
    ok = -1;
  }
}

// ------------------------------------------------------------------------------------------
// SELECCIONAR BOTON
// ------------------------------------------------------------------------------------------
void seleccionarBoton(){
   myNextion.sendCommand("page Escritorio");  
   nextion.listen();
   boton="";
   while(boton == ""){
    boton = myNextion.listen(); //check for message
   }
   Serial.println(boton);
   if (boton == "65 1 1 0 ffff ffff ffff") {
      Serial.println("ENTRADA");
      myNextion.sendCommand("p0.pic=7");
   }
}
    

// Cambiar a pagina Escritorio
// myNextion.sendCommand("page Escritorio");

// Cambiar foto
// myNextion.sendCommand("p0.pic=2");

// Capturar boton
// Hacer marca
// Si la marca se hace correctamente, mostrar pagina mensaje exitoso, sino tratar de nuevo



// ------------------------------------------------------------------------------------------
// LOOP
// ------------------------------------------------------------------------------------------
void loop(){
  leerCodigo();
  //seleccionarBoton();
}
