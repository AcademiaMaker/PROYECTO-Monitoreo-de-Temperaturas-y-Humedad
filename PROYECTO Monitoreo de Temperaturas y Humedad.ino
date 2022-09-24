/*  Programa de Monitoreo de Temperaturas y Humedad en 
    los Manglares de la Zona de Ciudad del Carmen Campeche
    
    Iniciativa: AcademiaMaker.org 
    Desarrollado: Jose Noe Castellanos Trejo
    Proyecto en GitHub:
    Fecha de creacion de proyecto: 21 - Sep - 2022

*/

#include <Wire.h>
#include <SoftwareSerial.h>
// rxPin_0 txPin_0 = 1,2
const byte rxPin_1 = 2;
const byte txPin_1 = 3;
const byte rxPin_2 = 4;
const byte txPin_2 = 5;

SoftwareSerial serialSecundario (rxPin_1, txPin_1, inverse_logic);
SoftwareSerial serialTerciario  (rxPin_2, txPin_2, inverse_logic);


#include <Adafruit_Sensor.h>                                                 //Librerias para el sensor DTH11 PCB o No PCB
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 6
#define DHTTYPE DHT11

DHT_Unified dht (DHTPIN, DHTTYPE);

float delay_event;

const int entrada1
const int entrada2
const int entrada3
const int entrada4

void setup() {
  Serial.begin(9600);

  pinMode(rxPin_1, INPUT);
  pinMode(txPin_1, OUTPUT);
  serialSecundario.begin(9600);
  
  pinMode(rxPin_2, INPUT);
  pinMode(txPin_2, OUTPUT);
  serialTerciario.begin(9600);

  inicializacionSensorDHT11();

  Wire.begin(8);                                      //Wire.begin(Direccion del dispositivo I2C, Maestro Opcional) - Inicializa Comunicacion I2C
  Wire.onRecive(eventoI2C);                           //Wire.onRecive(Funcion a realizar) -  Esta función registra una función que se llamará cuando un dispositivo periférico reciba una transmisión desde un dispositivo controlador.
}

void loop() {
  while(inicializacionPuertosSeries()){
    delay(delay_event);
    sensor_event_t lecturaDHT11;
    dht.temperature().getEvent(&lecturaDHT11);
    if(isnan(lecturaDHT11.temperature)){
      Serial.println("Error en la lectura de Temperatura")
    }
    else{
      Serial.println(lecturaDHT11.temperature);
    }
    dht.humidity().getEvent(&lecturaDHT11);
    if(isnan(lecturaDHT11.relative_humidity)){
      Serial.println("Error en la lectura de Humedad")
    }
    else{
      Serial.println(lecturaDHT11.relative_humidity)
    }
    
  }
  
  Serial.println("Fallo al inicializar el programa")
}

void inicializacionSensorDHT11(){
  Serial.println("Inicializando Sensor");
  dht.begin();
  sensor_t sensorDHT11;

  Serial.println("Incializando Sensor de Temperatura DHT11");
  dht.temperature().getSensor(&sensorDHT11);
  Serial.print("Nombre de Sensor:     ");   Serial.println(sensorDHT11.name);
  Serial.print("Tipo de Sensor:       ");   Serial.println(sensorDHT11.type);
  Serial.print("Resolucion de Sensor: ");   Serial.println(sensorDHT11.resolution);

  Serial.println("Incializando Sensor de Humedad DHT11");
  dht.humidity().getSensor(&sensorDHT11);
  Serial.print("Nombre de Sensor:     ");   Serial.println(sensorDHT11.name);
  Serial.print("Tipo de Sensor:       ");   Serial.println(sensorDHT11.type);
  Serial.print("Resolucion de Sensor: ");   Serial.println(sensorDHT11.resolution);

  delay_event = sensorDHT11.min_delay;
}

bool inicializacionPuertosSeries(){
  if (Serial.isListening() || serialSecundario.isListening() || serialTerciario.isListening()){
    Serial.println("Puerto Series Inicializados Correctamente");
    delay(10);
    return true;
  }
  
  else {
    Serial.println("Puerto Series No Inicializados Correctamente");
    delay(10);
    return false;
  }
}
void eventoI2C(){
  if(Wire.available() > 0){
    char dato_Recibido = Wire.read();
  }
}
