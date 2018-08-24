
char json_buffer1[512];
void reconnect(){
  while(!client.connected()){
    if(client.connect("Sergio"))
    {
 
        Serial.println("conectado");
      }else{
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        //Wait
        delay(5000);
      }
    }
  }
void json_HDC(){
  StaticJsonBuffer<512> jsonBuffer1;
  JsonObject& root=jsonBuffer1.createObject();
  root["sensor"]="HDC_CCS";
  JsonObject& data1=root.createNestedObject("COV");
    String CO2_string=String(CO2).c_str();
    String TVOC_string=String(TVOC).c_str();
    String HDC_temp_string=String(HDC_temp).c_str();
    String HDC_humidity_string=String(HDC_humidity).c_str();
    data1.set("CO2",CO2_string);
    data1.set("TVOC",TVOC_string);
    data1.set("HDC_temp",HDC_temp_string);
    data1.set("HDC_hum",HDC_humidity_string);
    root.printTo(json_buffer1,sizeof(json_buffer1));
    client.publish("nodo2",json_buffer1,true);
    Serial.println(json_buffer1);
}

