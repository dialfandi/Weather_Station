#include "Header.h"

void modbusRTU() {
  int buffersize = 7;
  float data[buffersize];
  uint8_t i;
  uint8_t j;
  uint8_t result;

  result = node.readHoldingRegisters(9, 7);

  if (result == node.ku8MBSuccess) {
    for (j = 0; j < buffersize; j++) {
      data[j] = node.getResponseBuffer(j);
//      Serial.print(j); Serial.print("->"); Serial.println(data[j]);

    }
    temp = (data[0]/100)-40;
    hum = data[1]/100;
    atmp = (int)data[2]/10;
    wspeed = data[3]/100;
    wdirect = data[4]/10;
    rain = data[5];
    iradian = (int)data[6]; 

//    Serial.println();
//    Serial.print("Temperature : ");
//    Serial.println(temp);
//    Serial.print("Humadity : ");
//    Serial.println(hum);
//    Serial.print("Atmosphere : ");
//    Serial.println(atmp);
//    Serial.print("Wind Speed : ");
//    Serial.println(wspeed);
//    Serial.print("Wind Direction : ");
//    Serial.println(wdirect);
//    Serial.print("Rainfall : ");
//    Serial.println(rain);
//    Serial.print("Iradian : ");
//    Serial.println(iradian); 
  }
}

void setData(){
  modbusRTU();
  
  char tmp[10];

  if(temp < 0)
    sprintf(tmp, "-%d", (int)temp);
  else
    sprintf(tmp, "%d", (int)temp);
  
  sprintf(dataIr, "Iradian        : %d W/M^2", iradian);
  sprintf(dataWs, "Wind Speed     : %d.%02d m/s", (int)wspeed, abs((int)(wspeed*100)%100));
  sprintf(dataWd, "Wind Direction : %d.%02d°", (int)wdirect, abs((int)(wdirect*100)%100));
  sprintf(dataTm, "Temperature    : %s.%02d °C", tmp, abs((int)(temp*100)%100));
  sprintf(dataRh, "Re Humadity    : %d.%02d %RH", (int)hum, abs((int)(hum*100)%100));
  sprintf(dataAp, "Atmospher Pres : %d hpa", atmp);
  sprintf(dataRf, "Rainfall       : %d.%02d mm/h", (int)rain, abs((int)(rain*100)%100));

//  sprintf(tagIr, "Iradian        : %d W/M^2", iradian);
//  sprintf(tagWs, "Wind Speed     : %d.%02d m/s", (int)wspeed, abs((int)(wspeed*100)%100));
//  sprintf(tagWd, "Wind Direction : %d.%02d°", (int)wdirect, abs((int)(wdirect*100)%100));
//  sprintf(tagTm, "Temperature    : %s.%02d °C", tmp, abs((int)(temp*100)%100));
//  sprintf(tagRh, "Re Humadity    : %d.%02d %RH", (int)hum, abs((int)(hum*100)%100));
//  sprintf(tagAp, "Atmospher Pres : %d hpa", atmp);
//  sprintf(tagRf, "Rainfall       : %d.%02d mm/h", (int)rain, abs((int)(rain*100)%100));

  sprintf(tagIr, "%d", iradian);
  sprintf(tagWs, "%d.%02d", (int)wspeed, abs((int)(wspeed*100)%100));
  sprintf(tagWd, "%d.%02d", (int)wdirect, abs((int)(wdirect*100)%100));
  sprintf(tagTm, "%s.%02d", tmp, abs((int)(temp*100)%100));
  sprintf(tagRh, "%d.%02d", (int)hum, abs((int)(hum*100)%100));
  sprintf(tagAp, "%d", atmp);
  sprintf(tagRf, "%d.%02d", (int)rain, abs((int)(rain*100)%100));
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("MQTT Connection...");
    if (client.connect("Mega2560Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected");
      kirim = 0;
    } else {
      Serial.print("Failed, rc = "); Serial.println(client.state());
      currentMillis = millis();
      if (currentMillis - previousMillisSD >= IntervalSend) {
        previousMillisSD = currentMillis;
        sendSD2();
        Serial.println("Send to SD Card");
        delay(1000);
        kirim = 1;
        ConnectGSM2();
      }
      // Wait 5 seconds before retrying
      delay(4000);
    }
  }
}

void sendData() {
  setData();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  client.publish(TAG1, tagIr);
  client.publish(TAG2, tagWs);
  client.publish(TAG3, tagWd);
  client.publish(TAG4, tagTm);
  client.publish(TAG5, tagRh);
  client.publish(TAG6, tagAp);
  client.publish(TAG7, tagRf);

  Serial.println("Send To Mqtt");
}

void sendSD(){
  setData();
  myFile = SD.open(filename, FILE_WRITE);

  if(myFile){
    countA++;
    myFile.println();
    myFile.println("================================================================");
    myFile.print("NO DATA : ");
    myFile.print(countA);
    myFile.println("  || MQTT STATUS -> CONNECTED -> SEND TO MQTT ||");
    myFile.println("================================================================");
    myFile.println(dataIr);
    myFile.println(dataWs);
    myFile.println(dataWd);
    myFile.println(dataTm);
    myFile.println(dataRh);
    myFile.println(dataAp);
    myFile.println(dataRf);
    myFile.println("================================================================");
    
    myFile.close();
  }
}

void sendSD2(){
  setData();
  myFile = SD.open(filename, FILE_WRITE);
  
  if(myFile){
    countB++;
    myFile.println();
    myFile.println("================================================================");
    myFile.print("NO DATA : ");
    myFile.print(countB);
    myFile.println("  || MQTT STATUS -> DISCONNECTED -> NOT SEND ||");
    myFile.println("================================================================");
    myFile.println(dataIr);
    myFile.println(dataWs);
    myFile.println(dataWd);
    myFile.println(dataTm);
    myFile.println(dataRh);
    myFile.println(dataAp);
    myFile.println(dataRf);
    myFile.println("================================================================");

    myFile.close();
  }
}

void SDCard(){
  Serial.println("Initializing SD card...");
  if (!SD.begin(pinSDCard)) {
    Serial.println("Initializing Failed!");
    while (1);
  }

  Serial.println("Initializing Done.");
  if (SD.exists(filename)) {
    Serial.println(filename + " Available");
  } else {
    Serial.println("Creating " + filename);
  }
  
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile){
    myFile.println();
    myFile.println("================================================================");
    myFile.println("================================================================");
    myFile.println("|||||||||                WEATHER STATION               |||||||||");
    myFile.println("|||||||||                    PT                        |||||||||");
    myFile.println("================================================================");
    myFile.println("|||||||||                SYSTEM C START                |||||||||");
    myFile.println("================================================================");
    myFile.println("================================================================");
    myFile.close();
  }
  
  delay(2000);
}

void ConnectGSM(){
  Serial.println("System Start");
  modem.restart();
  Serial.println(modem.getModemInfo());
  if(!modem.waitForNetwork())
  {
    Serial.println("Failed...");
    while(true);
  }
  Serial.println("Telkomsel");
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println("Failed...");
    while(true);
  }
  Serial.println(String(apn));
  delay(2000);
}

void ConnectGSM2(){
  Serial.println("System Start");
  modem.restart();
  Serial.println(modem.getModemInfo());
  if(!modem.waitForNetwork())
  {
    Serial.println("Failed...");
    if(kirim){
      sendSD2();
      Serial.println("Send to SD Card");
      delay(1000);
      reconnect();
    }
  }

  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println("Failed...");
    if(kirim){
        sendSD2();
        Serial.println("Send to SD Card");
        delay(1000);
        reconnect();
    }
  }
  else
    Serial.println(String(apn));
  
  delay(2000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(pinSDCard, OUTPUT);
  
  node.begin(slaveID, Serial2);
  
  ConnectGSM();
  SDCard();

  client.setClient(GsmClient);
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
//  modbusRTU();
//  delay(300);
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  if (currentMillis - previousMillisSend >= IntervalSend) {
    previousMillisSend = currentMillis;
    sendSD();
  }

  currentMillis = millis();
  if (currentMillis - previousMillisMqtt >= IntervalMqtt) {
    previousMillisMqtt = currentMillis;
    sendData();
  }
}
