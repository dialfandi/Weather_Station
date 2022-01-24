#define TINY_GSM_MODEM_SIM900

#include <Arduino.h>
#include <ModbusMaster.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SD.h>

ModbusMaster node1;
ModbusMaster node2;
ModbusMaster node3;

TinyGsm modem(Serial);
TinyGsmClient GsmClient(modem);
PubSubClient client;
File myFile;

#define slaveID 1

struct sensor
{
    float Speed;
    int Direc;
    
    char dataWs[50];
    char dataWd[50];
    
    char tagWs[40];
    char tagWd[40];
};
typedef struct sensor Sensor;

Sensor Wind1, Wind2, Wind3;

//Network details
const char apn[]  = "Internet";
const char user[] = "WAP";
const char pass[] = "WAP123";

// ====== MQTT ======
#define mqtt_server ".com"
#define mqtt_port 1883
#define mqtt_user ""
#define mqtt_password ""

#define TAG11 "TA-WS3-WD1" // Wind Direction 1
#define TAG12 "TA-WS3-WS1" // Wind Speed 1

#define TAG21 "TA-WS3-WD2" // Wind Direction 2
#define TAG22 "TA-WS3-WS2" // Wind Speed 2

#define TAG31 "TA-WS3-WD3" // Wind Direction 3
#define TAG32 "TA-WS3-WS3" // Wind Speed 3

// ====== SD Card ======
const int pinSDCard = 53;
String filename = "data_b.txt";

// ====== Global Variable ======
int count = 0, countA = 0, countB = 0;
long currentTime, lastTime;
char msg[20];
int snr1, snr2, snr3;
bool conn, kirim;

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
