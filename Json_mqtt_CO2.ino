//Write by Sergio Lopez 2018
//measurement CO2 and TVOC with CCS811 
//measuement Temperature and humidity with HDC1080
//I use a Node MCU
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include <Adafruit_Sensor.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "Adafruit_CCS811.h"
#include "ClosedCube_HDC1080.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif
////NODE MCU pins
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

////////
#define wifi_ssid "YourWiFi" 
#define wifi_password "**********"
#define mqtt_server "192.168.1.114" //put here your IP's MQTT broker
uint16_t mqtt_port=1883;
//
WiFiClient espClient;
PubSubClient client(mqtt_server,mqtt_port,espClient);
ClosedCube_HDC1080 hdc1080;
Adafruit_CCS811 ccs;
////////////////////////////////////////
long lastMsg=0;
//CCS811
int CO2=0;
int new_CO2=0;
float CCS_temp=0.0;
int TVOC=0;
//HDC1080
float HDC_temp=0.0;
float HDC_humidity=0.0;
//
float diff=1.00;
float diff1=0.1;
int ID=0;
int err=0;
int MS=0;
char dev_name[50];
////////////////////////////////////////////

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  //while (!Serial) continue;
  //setup sensors
  Wire.begin();
  //if(!sht31.begin(0x44) || bmp.begin()){
  //  Serial.println("SENSOR NO PRESENT");
  //}
  hdc1080.begin(0x40);
  //calibrate temperature sensor----
  while(!ccs.begin()){
    CCS_temp=ccs.calculateTemperature();
    ccs.setTempOffset(CCS_temp-25.0);
  }
  //setup wifi
  WiFi.begin(wifi_ssid,wifi_password);
  while(WiFi.status() !=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  reconnect();
  sprintf(dev_name,"ESP_%d",ESP.getChipId());

}
void read_sensor(){
  //read data from CCS811
  if(ccs.available()){
    CCS_temp=ccs.calculateTemperature();
    if(!ccs.readData()){
      CO2=ccs.geteCO2();
      TVOC=ccs.getTVOC();
    }
    else{
      Serial.println("ERROR:::");
      while(1);
    }
  }
  //Read Temp and humidity from HDC1080
  HDC_temp=hdc1080.readTemperature();
  HDC_humidity=hdc1080.readHumidity();
  delay(100);
}

void loop() {
  
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  MS=millis();
  if(MS-lastMsg>3000){
    lastMsg=millis();
    read_sensor();
//    json_SHT();
    json_HDC(); 
  }
}

