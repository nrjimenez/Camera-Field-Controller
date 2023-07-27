#ifndef internal_functions_h
#define internal_functions_h
#include "Arduino.h"
#include "icons.h"
#include "splash.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <AceButton.h>
using namespace ace_button;

extern int mainM;
extern int subM;
extern int subSubM;
extern int subSubSubM;
extern int pos;
//String identifyResponse; //this is received from master 100 identify command, it shows that cam is connected and camera ID
extern int cameraID; 
extern int newcameraID;;
extern bool connectionStatus; 
extern float temperature; 
extern unsigned long previousmillis;
extern bool passthroughmode;
extern Adafruit_SH1106G display;

  
unsigned char menuCursorLocation(int m, int s, int sS, int sSS, int p);
void menuDisplayFormat(String menuLabel, int numOfOptions, String option1, String option2, String option3, String option4, int indicatorValue);
void menuLocation(int m, int s, int sS, int sSS, int p);
void startUpScreen();
void updateConnectionStatus();
void checkCamConnectionStatus();
void updateCamID(int currentcamID, int newcamID);
int changeCamID(int cID, bool z);
void startUpCommands();
String saveSettings();
char sendCommand(int cID, String comm, int v);
void displayCamID(int cID);
void promptCommand();
void getConnectionStatus();
void getTemp();
void getCamID();
String masterCommand();
void indicator(int indic);


#endif