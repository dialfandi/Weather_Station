#include "Header.h"

void modbusRTU() {
  int buffersize = 2;
  float data[buffersize];
  uint8_t i;
  uint8_t j;
  uint8_t result;

  // ====== Sensor 1 ======
  result = node1.readHoldingRegisters(0, 2);
  if (result == node1.ku8MBSuccess) {
    Wind1.Speed = node1.getResponseBuffer(0)/10;
    Wind1.Direc = node1.getResponseBuffer(1);
  }

  // ====== Sensor 2 ======
  result = node2.readHoldingRegisters(0, 2);
  if (result == node2.ku8MBSuccess) {
    Wind2.Speed = node2.getResponseBuffer(0)/10;
    Wind2.Direc = node2.getResponseBuffer(1);
  }

  // ====== Sensor 3 ======
  result = node3.readHoldingRegisters(0, 2);
  if (result == node3.ku8MBSuccess) {
    Wind3.Speed = node3.getResponseBuffer(0)/10;
    Wind3.Direc = node3.getResponseBuffer(1);
  }
}

void setData(){
  modbusRTU();
  
  sprintf(Wind1.dataWs, "Wind Speed 1     : %d.%02d m/s", (int)Wind1.Speed, abs((int)(Wind1.Speed*100)%100));
  sprintf(Wind1.dataWd, "Wind Direction 1 : %d.%02d°", (int)Wind1.Direc, abs((int)(Wind1.Direc*100)%100));

  sprintf(Wind1.tagWs, "%d.%02d", (int)Wind1.Speed, abs((int)(Wind1.Speed*100)%100));
  sprintf(Wind1.tagWd, "%d.%02d", (int)Wind1.Direc, abs((int)(Wind1.Direc*100)%100));

  sprintf(Wind2.dataWs, "Wind Speed 2     : %d.%02d m/s", (int)Wind2.Speed, abs((int)(Wind2.Speed*100)%100));
  sprintf(Wind2.dataWd, "Wind Direction 2 : %d.%02d°", (int)Wind2.Direc, abs((int)(Wind2.Direc*100)%100));

  sprintf(Wind2.tagWs, "%d.%02d", (int)Wind2.Speed, abs((int)(Wind2.Speed*100)%100));
  sprintf(Wind2.tagWd, "%d.%02d", (int)Wind2.Direc, abs((int)(Wind2.Direc*100)%100));

  sprintf(Wind3.dataWs, "Wind Speed 3     : %d.%02d m/s", (int)Wind3.Speed, abs((int)(Wind3.Speed*100)%100));
  sprintf(Wind3.dataWd, "Wind Direction 3 : %d.%02d°", (int)Wind3.Direc, abs((int)(Wind3.Direc*100)%100));

  sprintf(Wind3.tagWs, "%d.%02d", (int)Wind3.Speed, abs((int)(Wind3.Speed*100)%100));
  sprintf(Wind3.tagWd, "%d.%02d", (int)Wind3.Direc, abs((int)(Wind3.Direc*100)%100));
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("Mega2560Client", mqtt_user, mqtt_password)) {
      kirim = 0;
    } else {
      currentMillis = millis();
      if (currentMillis - previousMillisSD >= IntervalSend) {
        previousMillisSD = currentMillis;
        sendSD2();
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

  client.publish(TAG11, Wind1.tagWd);
  client.publish(TAG12, Wind1.tagWs);

  client.publish(TAG21, Wind2.tagWd);
  client.publish(TAG22, Wind2.tagWs);

  client.publish(TAG31, Wind3.tagWd);
  client.publish(TAG32, Wind3.tagWs);
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
    myFile.println(Wind1.dataWs);
    myFile.println(Wind1.dataWd);
    myFile.println(Wind2.dataWs);
    myFile.println(Wind2.dataWd);
    myFile.println(Wind3.dataWs);
    myFile.println(Wind3.dataWd);
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
    myFile.println(Wind1.dataWs);
    myFile.println(Wind1.dataWd);
    myFile.println(Wind2.dataWs);
    myFile.println(Wind2.dataWd);
    myFile.println(Wind3.dataWs);
    myFile.println(Wind3.dataWd);
    myFile.println("================================================================");

    myFile.close();
  }
}

void SDCard(){
  if (!SD.begin(pinSDCard)) {
    while (1);
  }
  
  myFile = SD.open(filename, FILE_WRITE);
  if(myFile){
    myFile.println();
    myFile.println("================================================================");
    myFile.println("================================================================");
    myFile.println("|||||||||                WEATHER STATION               |||||||||");
    myFile.println("|||||||||                    PT                        |||||||||");
    myFile.println("================================================================");
    myFile.println("|||||||||                SYSTEM B START                |||||||||");
    myFile.println("================================================================");
    myFile.println("================================================================");
    myFile.close();
  }
  
  delay(2000);
}

void ConnectGSM(){
  modem.restart();
  if(!modem.waitForNetwork())
  {
    while(true);
  }

  if (!modem.gprsConnect(apn, user, pass))
  {
    while(true);
  }
  delay(2000);
}

void ConnectGSM2(){
  modem.restart();
  if(!modem.waitForNetwork())
  {
    if(kirim){
      sendSD2();
      delay(1000);
      reconnect();
    }
  }

  if (!modem.gprsConnect(apn, user, pass))
  {
    if(kirim){
        sendSD2();
        delay(1000);
        reconnect();
    }
  }
  
  delay(2000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  pinMode(pinSDCard, OUTPUT);
  
  node1.begin(slaveID, Serial1);
  node2.begin(slaveID, Serial2);
  node3.begin(slaveID, Serial3);
  
  ConnectGSM();
  SDCard();

  client.setClient(GsmClient);
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
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
