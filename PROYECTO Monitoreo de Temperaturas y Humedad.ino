
/**************************************************************************/
/*!
    Programa de Monitoreo de Temperaturas y Humedad en los Manglares de la 
    Zona de Ciudad del Carmen Campeche
    
    Iniciativa: AcademiaMaker.org 
    Desarrollado: Jose Noe Castellanos Trejo
    Proyecto en GitHub:
    Fecha de creacion de proyecto: 21 - Sep - 2022


    Con el desarrollo de esta implementacion tecnologica utilizando
    software y hardware de codigo abierto, se planea mejorar la
    monitorizacion de los manglares y humedales de la zona de campeche, 
    que es espacio protegido por medio de la tecnologia IOT, Internet de 
    las Cosas, e implementar iniicativas para cuidar de nuestros recursos
*/
/**************************************************************************/

/*
    Librerias para crear puertos series por Software, estos puerto series
    tienen sus limitaciones, conoce en www.arduino.cc mas info sobre la
    libreria Software Serial
*/
#include <SoftwareSerial.h>                                            

//  Declaraciones para las librerias de Sofware Serial
//  Los pines 0 y 1 son predeterminados por arduino para comunicacio serie
const byte rxPin_2 = 2;
const byte txPin_2 = 3;
const byte rxPin_3 = 4;
const byte txPin_3 = 5;

SoftwareSerial serialAuxiliar2(rxPin_2, txPin_2);
SoftwareSerial serialAuxiliar3(rxPin_3, txPin_3);

/*
    Librerias para unificar la toma de resultados, y la conversion
    de los datos de los sensores al SI (Sistema Internacional) con ello
    englobar los datos presentados por los sensores de diferentes marcas
    en uno solo. Mas informacion en www.adafruit.com
*/
#include <Adafruit_Sensor.h>                                            
#include <DHT.h>
#include <DHT_U.h>

//  Declaraciones para las librerias de Adafruit Sensor Unified
//  Librerias para el sensor DTH11 DHT21 DHT22
#define DHTPIN 6
#define DHTTYPE DHT11

DHT_Unified dhtSensor (DHTPIN, DHTTYPE);

float delay_event_dht;

/*
    Librerias para unificar la toma de resultados, y la conversion
    de los datos de los sensores al SI (Sistema Internacional) con ello
    englobar los datos presentados por los sensores de diferentes marcas
    en uno solo. El sensor de presion barometris BMP085 utiliza la 
    comunicacion I2C por lo cual se necesita anexar la libreria Wire para
    mas informacion en www.arduino.cc y www.adafruit.com

    La libreria Wire es utilizada por la libreria del BMP085 por lo que 
    no sera vista en la programacion. 

    Conexiones del Sensor BMP085
    ===========

    Conectar SCL a Analogico 5
    Conectar SDA a Analogico 4
    Conectar VDD a 3.3V
    Conectar GROUND a Tierra Comun
*/
#include <Wire.h>                                                      
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmpSensor(10085);

float delay_event_bmp;

//  Variables Globales para la ejecucion del programa
bool estadoPuertosSeries;

//  Constantes de tiempo de espera para la ejecucion del programa
const int tiempoDelay = 500;
const int tiempoReboot = 5000;
const int entrada4 = 0;

//  Funciones de inicialziacion de los sensores y puertos series
bool inicializacionPuertosSeries(){
  bool estadoPuertoSeriePrincipal;
  bool estadoPuertoSerieAuxiliar;

  Serial.begin(9600);
  serialAuxiliar2.begin(9600);
  serialAuxiliar3.begin(9600);

  while(!Serial){
//  Se implementara una pantalla para poder ver los status del error
    Serial.println("Inicializando Puerto Serie");
    delay(tiempoDelay);

    if(!Serial){
        Serial.print("ERROR FATAL");
        Serial.println("Puerto Serie - Error: Fallo al Iniciar");
    }
    else if(Serial){
      Serial.println("INICIO COMPLETADO");
      delay(tiempoDelay);
      Serial.println("Puerto Serie - MODE: inicializado correctamente");
      delay(tiempoDelay);
      estadoPuertoSeriePrincipal = true;
      break;
    }
    Serial.println("Reiniando Puerto Serie");
    delay(tiempoDelay);
  }


  if (!serialAuxiliar2 && !serialAuxiliar3)
  {
    estadoPuertoSerieAuxiliar = false;
    Serial.println("Puertos Series auxiliares - Error: Fallo al Iniciar");
  }
  else if (serialAuxiliar2 && serialAuxiliar3) {
    estadoPuertoSerieAuxiliar = true;
    if (estadoPuertoSeriePrincipal && estadoPuertoSeriePrincipal){
      return true;
    }
    else {
      return false;
    }
  }
}

//  Funcion para inicializar los sensores de Temperatura y Humedad
void inicializacionSensorDHT11(){                                   
  Serial.println("Inicializando Sensor");
  delay(tiempoDelay);

  dhtSensor.begin();
  sensor_t sensorDHT11;

  Serial.println("Incializando Sensor de Temperatura DHT11");
  dhtSensor.temperature().getSensor(&sensorDHT11);
  Serial.print("Nombre de Sensor:     ");   Serial.println(sensorDHT11.name);
  Serial.print("Tipo de Sensor:       ");   Serial.println(sensorDHT11.type);
  Serial.print("Resolucion de Sensor: ");   Serial.println(sensorDHT11.resolution);

  Serial.println("Incializando Sensor de Humedad DHT11");
  dhtSensor.humidity().getSensor(&sensorDHT11);
  Serial.print("Nombre de Sensor:     ");   Serial.println(sensorDHT11.name);
  Serial.print("Tipo de Sensor:       ");   Serial.println(sensorDHT11.type);
  Serial.print("Resolucion de Sensor: ");   Serial.println(sensorDHT11.resolution);

  delay_event_dht = sensorDHT11.min_delay;
}

// Funcion para inicializar los sensores de Temperatura y Humedad
void inicializacionSensorBMP085(){                                    
  Serial.println("Inicializando Sensor");
  delay(tiempoDelay);

  sensor_t sensorBMP085;
  
  Serial.println("Incializando Sensor de Temperatura DHT11");
  bmpSensor.getSensor(&sensorBMP085);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensorBMP085.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensorBMP085.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensorBMP085.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensorBMP085.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensorBMP085.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensorBMP085.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");

  delay_event_bmp = sensorBMP085.min_delay;
}

//  Lectura del sensor DHT11 Humedad y Temperatura
void lecturaDHT11 (){
  sensors_event_t readDHT11;
  dhtSensor.temperature().getEvent(&readDHT11);

  if(isnan(readDHT11.temperature)){
    Serial.println("Error: lectura de Temperatura");
    delay(delay_event_bmp);
  }
  else{
    Serial.println("----------------SENSOR: DHT11--------------------");
    Serial.print("Temperatura:    ");   Serial.print(readDHT11.temperature);  Serial.println(" ºC");
    Serial.println("-------------------------------------------------");
    delay(delay_event_dht);
  }
  
  dhtSensor.humidity().getEvent(&readDHT11);

  if(isnan(readDHT11.relative_humidity)){
    Serial.println("Error: lectura de Humedad");
    delay(delay_event_dht);
  }
  else{
    Serial.println("----------------SENSOR: DHT11--------------------");
    Serial.print("Humedad:    ");   Serial.print(readDHT11.relative_humidity);  Serial.println(" %HR");
    Serial.println("-------------------------------------------------");
    delay(delay_event_dht);
  }
}

//  Lecutura del Sensor BMP085 de presion barometrica
void lecturaBMP085(){
  sensors_event_t readBMP085;
  bmpSensor.getEvent(&readBMP085);

  if (isnan(readBMP085.pressure)){
    Serial.println("Error: Lectura de Presion Barometrica");
    delay(delay_event_bmp);
  }
  else{
    Serial.println("----------------SENSOR: BMP085--------------------");
    Serial.print("Presion Barometrica:    ");   Serial.print(readBMP085.pressure);  Serial.println(" hPa");
    Serial.println("--------------------------------------------------");
    delay(delay_event_bmp);
  }
  
  
}
//  Configuracion del Arduino
void setup() {                                                      
  estadoPuertosSeries = inicializacionPuertosSeries();
  inicializacionSensorDHT11();
  inicializacionSensorBMP085();                                     
}

//  Ejecucion del programa
void loop() {
  while(!estadoPuertosSeries){
    lecturaDHT11();
    lecturaBMP085();
    delay(tiempoDelay);
  }
  Serial.println("Fallo al inicializar el programa");
  delay(tiempoReboot);
}
