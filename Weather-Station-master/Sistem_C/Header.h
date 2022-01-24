#define TINY_GSM_MODEM_SIM900

#include <Arduino.h>
#include <ModbusMaster.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SD.h>

ModbusMaster node;

TinyGsm modem(Serial1);
TinyGsmClient GsmClient(modem);
PubSubClient client;
File myFile;

#define slaveID 255

//Network details
const char apn[]  = "";
const char user[] = "";
const char pass[] = "";

// ====== MQTT ======
#define mqtt_server ""
#define mqtt_port 
#define mqtt_user ""
#define mqtt_password ""

#define TAG1 "TA-WS2-IR" // Iradian
#define TAG2 "TA-WS2-WS" // Wind Speed
#define TAG3 "TA-WS2-WD" // Wind Direction
#define TAG4 "TA-WS2-AT" // Temperature
#define TAG5 "TA-WS2-RH" // Relative Humadity
#define TAG6 "TA-WS2-AP" // Atmospheric Pressure
#define TAG7 "TA-WS2-RF" // Rainfal

// ====== SD Card ======
const int pinSDCard = 53;
String filename = "data_c.txt";

// ====== Global Variable ======
int count = 0, countA = 0, countB = 0;
long currentTime, lastTime;
char msg[20];
int snr1, snr2, snr3;
bool conn, kirim;

float wspeed, wdirect, temp, hum, rain;
int iradian, atmp;

char dataIr[50];
char dataWs[50];
char dataWd[50];
char dataTm[50];
char dataRh[50];
char dataAp[50];
char dataRf[50];

char tagIr[40];
char tagWs[40];
char tagWd[40];
char tagTm[40];
char tagRh[40];
char tagAp[40];
char tagRf[40];

//======= Timing ========
unsigned long currentMillis;
unsigned long previousMillisLCD = 0;
unsigned long previousMillisSend = 0;
unsigned long previousMillisSD = 0;
unsigned long previousMillisCon = 0;
unsigned long previousMillisMqtt = 0;
const int IntervalLCD = 1000; //Send data setiap 3 detik sekali
const int IntervalMqtt = 500;
const int IntervalSend = 2000;
const int IntervalCon = 30000;
