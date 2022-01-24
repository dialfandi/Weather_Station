#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>

#define wifi_ssid "MIT"
#define wifi_password "pens1234"

//#define TAG1 "TA-WS1-IR" // Iradian
//#define TAG2 "TA-WS1-WS" // Wind Speed
//#define TAG3 "TA-WS1-WD" // Wind Direction

#define mqtt_server ""
#define mqtt_port 
#define mqtt_user ""
#define mqtt_password ""

#define TAG1 "/proj/TA-WS1-IR" // Iradian
#define TAG2 "/proj/TA-WS1-WS" // Wind Speed
#define TAG3 "/proj/TA-WS1-WD" // Wind Direction

WiFiClient espClient;
PubSubClient client;

// ====== Global Variable ======
int count, bcount;
long currentTime, lastTime;
int iradian;
int windDir;
int windSpeed;
String data;
String Send;

int counter = 0;
int lastIndex = 0;

String pieces[3];

char dataIr[60];
char dataWs[60];
char dataWd[60];

char TimeNow[30];

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 3600;

//======== Timing ========
unsigned long currentMillis;
unsigned long previousMillisSend = 0;
unsigned long previousMillisR = 0;
unsigned long previousMillisSD = 0;
const int IntervalSend = 1000;
