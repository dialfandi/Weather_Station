#include "Header.h"

void modbusRTU() {
//  uint8_t result;
//
//  // ====== Wind Sensor ======
//  result = node1.readHoldingRegisters(0, 2);
//  //  Serial.print("Result 1");
//  //  Serial.println(result);
//
//  if (result == node1.ku8MBSuccess) {
//    windSpeed = node1.getResponseBuffer(0) / 10;
//    windDir = node1.getResponseBuffer(1);
//  }
//
//  // ====== Iradian Sensor ======
//  result = node2.readHoldingRegisters(0, 1);
//  //  Serial.print("Result 2");
//  //  Serial.println(result);
//
//  if (result == node2.ku8MBSuccess) {
//    iradian = node2.getResponseBuffer(0);
//  }
  iradian = rand() % 10;
  windSpeed = rand() % 10;
  windDir = rand() % 10;
}

void setData() {
  modbusRTU();
  
  sprintf(dataIr, "TA-WS1-IR : %d W/m^2", iradian);
  sprintf(dataWs, "TA-WS1-WS : %d m/s", windSpeed);
  sprintf(dataWd, "TA-WS1-WD : %d°", windDir);
}

void sendSD() {
  setData();
  myFile = SD.open(filename, FILE_WRITE);

  if (myFile) {
    count++;
    myFile.println();
    myFile.println("==================================");
    //    myFile.print(TimeNow);
    myFile.print("NO DATA : ");
    myFile.println(count);
    myFile.println("==================================");
    myFile.println(dataIr);
    myFile.println(dataWs);
    myFile.println(dataWd);
    myFile.println("==================================");

    myFile.close();
  }
}

void SDCard() {
  if (!SD.begin(pinSDCard)) {
    while (1);
  }

  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    myFile.println();
    myFile.println("==================================================");
    myFile.println("==================================================");
    myFile.println("||                WEATHER STATION               ||");
    myFile.println("||                                              ||");
    myFile.println("==================================================");
    myFile.println("||                SYSTEM A START                ||");
    myFile.println("==================================================");
    myFile.println("==================================================");
    myFile.close();
  }

  delay(2000);
}

void sendtoESP() {
  setData();

  char data[10];

  sprintf(data, "%d,%d,%d", iradian, windSpeed, windDir);

  Serial.println(data);
  //  Serial.println(data);
}

void ReceiceTime() {
  while (Serial.available() > 0) {
    delay(10);
    char d = Serial.read();
    data += d;
  }

  if (data.length() > 0) {
    Serial.println(data);
    TimeNow = data;
    data = "";
  }

  Serial.println(TimeNow);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial1.begin(9600);
  //Serial2.begin(9600);
  //Serial3.begin(9600);
  pinMode(pinSDCard, OUTPUT);

  //node1.begin(slaveID1, Serial1);
  //node2.begin(slaveID2, Serial2);
  Serial.print("FHSAJ");
  SDCard();
}

void loop() {
  // put your main code here, to run repeatedly:
  //  ReceiceTime();

  
  currentMillis = millis();
  if (currentMillis - previousMillisSend >= IntervalSend) {
    previousMillisSend = currentMillis;
    sendSD();
  }
  //ReceiceTime();
  sendtoESP();
}
