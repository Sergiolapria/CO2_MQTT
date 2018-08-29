void read_hdc(){
  hdc_temp=hdc1080.readTemperature();
  hdc_humidity=hdc1080.readHumidity();
}
void read_ccs(){
  if(ccs.available()){
    //dynamic temperature and humidity compensation
    ccs.setEnvironmentalData(hdc_humidity,hdc_temp);
    if(!ccs.readData()){
      ccs_CO2=ccs.geteCO2();
      ccs_TVOC=ccs.getTVOC();
    }
    else{
      Serial.println("ERROR");
      while(1);
    }
  }
}

