#define TINY_GSM_MODEM_SIM900

#include <ModbusMaster.h>
#include <SD.h>

ModbusMaster node1;
ModbusMaster node2;

int slaveID1 = 1;
int slaveID2 = 1;

File myFile;

// ====== SD Card ======
const int pinSDCard = 53;
String filename = "data_a.txt";

// ====== Global Variable ======
int count = 0;
long currentTime, lastTime;
int iradian;
int windDir;
int windSpeed;

char dataIr[50];
char dataWs[50];
char dataWd[50];
char dataSend[50];

String TimeNow;
String data;

//======== Timing ========
unsigned long currentMillis;
unsigned long previousMillisSend = 0;
unsigned long previousMillisSD = 0;
const int IntervalSend = 500;
