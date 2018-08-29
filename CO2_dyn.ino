
////Write by Sergio Lopez(@SergioPria) 2018
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <WiFiClient.h>
#include <Adafruit_CCS811.h>
#include "ClosedCube_HDC1080.h"
//////
#define wifi_ssid "XXXXXXX"
#define wifi_password "xxxxxx"
#define mqtt_server "192.168.1.114"
uint16_t mqtt_port=1883;
//
WiFiClient espClient;
PubSubClient client(mqtt_server,mqtt_port,espClient);
ClosedCube_HDC1080 hdc1080;
Adafruit_CCS811 ccs;
///
 int hdc_humidity=0;
 double hdc_temp=0.0;
 int ccs_CO2=0;
 int ccs_TVOC=0;
 //
 long ms=0;
 long last_sample=0;

void setup() {
  //Setup sensors
  Wire.begin();
  hdc1080.begin(0x40);
  while(!ccs.begin()){
    ccs.setDriveMode(2);
    /*
      1--CCS811_DRIVE_MODE_IDLE
      2--CCS811_DRIVE_MODE_1SEC
      3--CCS811_DRIVE_MODE_10SEC
      4--CCS811_DRIVE_MODE_60SEC
      5--CCS811_DRIVE_MODE_250MS
    */
    delay(1);
  }
  //setup WiFi
  Serial.begin(9600);
  WiFi.begin(wifi_ssid,wifi_password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected:");
  Serial.print("IP Address");
  Serial.println(WiFi.localIP());
  reconnect();
}
void loop() {
    if(!client.connected()){
      reconnect();
    }
    client.loop();
    ms=millis();
    if(ms-last_sample>10000){
        last_sample=millis();
        read_hdc();
        read_ccs();
        //CheckBound?
        send_json();
    }
}

