char json_buffer[264];
void reconnect(){
  while(!client.connected()){
    if(client.connect("Sergio")){
      Serial.println("Conectado");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 sec");
      delay(5000);
    }
  }
}
void send_json(){
  StaticJsonBuffer<264> jsonBuffer;
  JsonObject& root=jsonBuffer.createObject();
  root["sensor"]="HDC_CCS";
  JsonObject& data1=root.createNestedObject("COV");
      String CO2_string=String(ccs_CO2).c_str();
      String TVOC_string=String(ccs_TVOC).c_str();
      String humidity_string=String(hdc_humidity).c_str();
      String temperature_string=String(hdc_temp).c_str();
/////////////////////////////////////////////////
      data1.set("CO2",CO2_string);
      data1.set("TVOC",TVOC_string);
      data1.set("Humidity",humidity_string);
      data1.set("Temperature",temperature_string);
  root.printTo(json_buffer,sizeof(json_buffer));
  client.publish("nodo2",json_buffer,true);
  Serial.println(json_buffer);
}
