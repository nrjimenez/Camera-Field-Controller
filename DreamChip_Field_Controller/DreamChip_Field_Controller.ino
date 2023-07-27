int mainM = 0;
int subM = 0;
int subSubM = 0;
int subSubSubM = 0;
int pos = 0;
//String identifyResponse; //this is received from master 100 identify command, it shows that cam is connected and camera ID
int cameraID; 
int newcameraID = cameraID;
bool connectionStatus; 
float temperature; 
unsigned long previousmillis = 0;
bool passthroughmode = 0;


#include "src/internal_functions.h"


const int BUTTON_UP = 6;
const int BUTTON_DOWN = 5;
const int BUTTON_RIGHT = 4;
const int BUTTON_LEFT = 3;
const int BUTTON_SELECT = 7;

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C

#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height
#define OLED_RESET -1   //  
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//AceButton assignments
AceButton left(BUTTON_LEFT);
AceButton right(BUTTON_RIGHT);
AceButton down(BUTTON_DOWN);
AceButton up(BUTTON_UP);
AceButton select(BUTTON_SELECT);

void handleEvent(AceButton*, uint8_t, uint8_t);





void setup(){
  Serial1.begin(115200); //serial communication to camera
  Serial.begin(115200);
  Serial1.setTimeout(100);
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); //OLED start up

  pinMode(2, OUTPUT); //DE & RE, when put on HIGH you can send commands. LOW is for reading

  pinMode(BUTTON_LEFT, INPUT_PULLUP); //LEFT
  pinMode(BUTTON_RIGHT, INPUT_PULLUP); //RIGHT
  pinMode(BUTTON_DOWN, INPUT_PULLUP); //DOWN
  pinMode(BUTTON_UP, INPUT_PULLUP); //UP
  pinMode(BUTTON_SELECT, INPUT_PULLUP); //SELECT

  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

  display.clearDisplay();

  display.drawBitmap(0, 0,  SPLASH, 128, 64, SH110X_WHITE); 
  display.display();
  delay(1000);
  display.clearDisplay();
  
  startUpCommands(); //master command, camid, prompt, temp
  
  previousmillis = millis();
 
}

void loop() {
  //checks for whenn a button gets pressed
  left.check();
  right.check();
  up.check();
  down.check();
  select.check();
  if(passthroughmode){
    if(Serial.available()){
      digitalWrite(2, HIGH); //write
       Serial1.write(Serial.read());
      while(!(USART1.STATUS & USART_TXCIF_bm)){
          //Serial.print("waiting");
      }
      digitalWrite(2, LOW);
    }
    if(Serial1.available()){
      Serial.write(Serial1.read());
    }
  }else{
    if(millis() - previousmillis > 2000){
    previousmillis = millis();
    bool previousConnectionStatus = connectionStatus;
    checkCamConnectionStatus();
    if(previousConnectionStatus != connectionStatus){
      updateConnectionStatus();
      display.display();
    }
  }  
  }

}

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState){

  switch(eventType){
    case AceButton::kEventPressed:

    if(button->getPin() == BUTTON_RIGHT){ //increases position, which will take you from main -> sub -> subsub -> subsubsub
      if(mainM == 1 ){
        if(subM == 0){
          if(pos >= 0 && pos < 3){
            pos++;   
          }
        }else if(subM == 1 || subM == 2){
          if(pos >= 0 && pos < 2){
            pos++;
          }
        }
      }else if(mainM == 0 || mainM == 2 || mainM == 3){
        if(pos == 0){
            pos++;
          }
      }
    }else if(button->getPin() == BUTTON_LEFT){ //decreases position, which will take you from subsubsub -> subsub -> sub -> main
      if(mainM == 1){
        if(subM == 0){
          if(pos > 0 && pos <= 3){
            pos--;
            
          }
        }else if(subM == 1 || subM == 2){
          if(pos > 0 && pos <= 2){
            pos--;
          }
        }
      }else if(mainM == 0 || mainM == 2){
        if(pos == 1){
            pos--;
          }
      }
      if( mainM == 3 && pos == 1){
        passthroughmode = 0;
        pos--;
      }
    }else if(button->getPin() == BUTTON_DOWN){ //scrolls down on any of the menus displayed
        if(pos == 0){ //up main menu
        mainM++;
        if(mainM > 3){ 
          mainM = 0;
        }
        }else if(pos == 1){
          if(mainM == 1){
          subM++;
          if(subM > 3){
            subM = 0;
          }
        }
      }else if(pos == 2){
        if(subM == 0){ // AUTO/MANUAL SELECTION MENU
          subSubM++;
          if(subSubM > 1){
            subSubM = 0;
          }
        }
        if(subM == 1){ //ROTATION MENU
          subSubM++;
          if(subSubM > 3){
            subSubM = 0;
          }
        }
      }else if(pos == 3){
        if(subSubM == 0){ //AUTO MENU
          subSubSubM++;
          if(subSubSubM > 2){
            subSubSubM = 0;
          }
        }
        if(subSubM == 1){ //MANUAL MENU
          subSubSubM++;
          if(subSubSubM > 2){
            subSubSubM = 0;
          }
        }
      }
      if(mainM == 1 && subM == 2 && pos == 2){ //decreases the cameraid value
        newcameraID = changeCamID(newcameraID, 0);
      }
    
  
    }else if(button->getPin() == BUTTON_UP){ //scrolls up on menu displays
      if(pos == 0){ //main menu
        mainM--;
        if(mainM < 0){
          mainM = 3;
        }
      }else if(pos == 1){ //sub menu
        if(mainM == 1){ //camera settings sub menu
          subM--;
          if(subM < 0){
            subM = 3;
          }
        }
      }else if(pos == 2){ //subsub menus
        if(subM == 0){ //auto/manual setting sub sub menu
          subSubM--;
          if(subSubM < 0){
            subSubM = 1;

          }
        }
        if(subM == 1){ //rotation settings sub sub menu
          subSubM--;
          if(subSubM < 0){
            subSubM = 1;
          }
        }
      }else if(pos == 3){ //subsubsub menus
        if(subSubM == 0){ //auto menu
          subSubSubM--;
          if(subSubSubM < 0){
            subSubSubM = 2;
          }
        }
        if(subSubM == 1){ //manual menu
          subSubSubM--;
          if(subSubSubM < 0){
            subSubSubM = 2;
          }
        }
      }
      if(mainM == 1 && subM == 2 && pos == 2){ //increases value of camera id
         newcameraID = changeCamID(newcameraID, 1); 
        }

    }else if(button->getPin() == BUTTON_SELECT){ 
       if(mainM == 0 && pos == 1){ //updates Camera Information menu
        getCamID();
        getTemp();
       }
      if(mainM == 1 && subM == 2 && pos == 2){ //Saves camera ID
        updateCamID(cameraID, newcameraID);
      }

      if(mainM == 2 && pos == 1){ //updates Connection menu
        getConnectionStatus();
      }
      
      if(mainM == 3 && pos == 0){

      }
      if(mainM == 1 && subM == 0  && pos == 3){ //subSub menu of Auto/Manual
        if(subSubM == 0){ //AUTO MENU
          if(subSubSubM == 0){ 
            sendCommand(cameraID, "aec", 1); //auto exposure on
            sendCommand(cameraID, "awb", 1); //auto white balance on
            saveSettings();
          }else if(subSubSubM == 1){
            sendCommand(cameraID, "aec", 1); //auto exposure on
            saveSettings();
          }else if(subSubSubM == 2){
            sendCommand(cameraID, "awb", 1); //auto white balance on
            saveSettings();
          }
        }
        if(subSubM == 1){ //MANUAL MENU
          if(subSubSubM == 0){ 
            sendCommand(cameraID, "aec", 0); //manual exposure
            sendCommand(cameraID, "awb", 0); //manual white balance
            saveSettings();
          }else if(subSubSubM == 1){
            sendCommand(cameraID, "aec", 0); //manual exposure
            saveSettings();
          }else if(subSubSubM == 2){
            sendCommand(cameraID, "awb", 0); //manual white balance
            saveSettings();
          }
        }
      }
      if(mainM == 1 && subM == 1 && pos == 2){//subSub menu for Rotation
        if(subSubM == 0){
          sendCommand(cameraID, "flip", 3); //flip 180 degrees
        }else if(subSubM == 1){
          sendCommand(cameraID, "flip", 1); //flig vertical
        }else if(subSubM == 2){
          sendCommand(cameraID, "flip", 2); //flip horizontal
        }else if(subSubM == 3){
          sendCommand(cameraID, "flip", 0); //reset flip
        }
      }
      if(mainM == 1 && subM == 3 && pos == 2){//save button
        saveSettings();
      }
    }
     menuLocation(mainM, subM, subSubM, subSubSubM, pos); //updates menu whenever a button is pressed
    indicator(menuCursorLocation(mainM, subM, subSubM, subSubSubM, pos)); //updates indicator whenever a button is pressed
  }
}






































