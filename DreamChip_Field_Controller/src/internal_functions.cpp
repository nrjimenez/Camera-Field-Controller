#include "internal_functions.h"
#include "Arduino.h"

const unsigned char cursorPosition[192] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  0, 0, 0, 1, 1, 1, 1, 8, 8, 8, 8, 8,
  0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3,
  7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  0, 1, 2, 0, 1, 2, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
  };

unsigned char menuCursorLocation(int m, int s, int sS, int sSS, int p){
  if(p == 0){return m;}
  if(p == 1 && (m == 0 || m == 2 || m == 3)){return 8;}
  unsigned char index = 0;
  index = ((p-1)*48) + (s*12) + (sS*3) + sSS;  
  return cursorPosition[index];
}

//template for Menu when it has normal options and no special functions
void menuDisplayFormat(String menuLabel, int numOfOptions, String option1, String option2, String option3, String option4, int indicatorValue){
      display.clearDisplay();
      updateConnectionStatus();
      display.setCursor(0,0);
      displayCamID(cameraID);
      display.setTextSize(1);
      display.setCursor(0,10);
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.println(menuLabel);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(10,20);
      display.println(option1);
      display.setCursor(10,30);
      display.println(option2);
      if(numOfOptions > 2){
      display.setCursor(10,40);
      display.println(option3);
      }
      if(numOfOptions > 3){
      display.setCursor(10,50);
      display.println(option4);
      }
      display.display();
}

void menuLocation(int m, int s, int sS, int sSS, int p){
  //main menu
  if(p == 0){
    menuDisplayFormat("Main Menu", 4, "Camera Information", "Camera Settings", "Connection Status", "Pass-through mode", mainM);
  }
  if(m == 3 && p == 1){
    display.clearDisplay();
    Serial1.flush();
    Serial.flush();
    display.setCursor(0,0);
    displayCamID(cameraID);
    display.setTextSize(1);
    display.setCursor(0,10);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
    display.println("Pass-through mode");
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(10, 20);
    passthroughmode = 1;
    

    display.display();
  }

  //camera information sub menu
  if(m == 0 && p == 1){
    display.clearDisplay();
    updateConnectionStatus();
    display.setCursor(0,0);
    displayCamID(cameraID);
    display.setTextSize(1);
    display.setCursor(0,10);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
    display.println("Camera Information");
    display.setTextColor(SH110X_WHITE);
    display.setCursor(10,20);
    display.print("Temperature: ");
    display.print(temperature);
    display.setCursor(10,30);
    display.print("Camera ID: ");
    display.println(cameraID);
    display.setCursor(10,40);
    display.println("Serial #"); //figure out what to put here
    //indicator(menuCursorLocation(m, s, sS, sSS, p));
    display.display();
  }

  //connection status sub menu
  if(m == 2 && p == 1){
    display.clearDisplay();
    updateConnectionStatus();
    display.setCursor(0,0);
    displayCamID(cameraID);
    display.setTextSize(1);
    display.setCursor(0,10);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
    display.println("Connection Status");
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(10, 20);
    display.println("Press SEL to check");
    if(connectionStatus == 0){
      display.drawBitmap(60, 40, smileBig, 16, 12, SH110X_WHITE);
    }else{
      display.drawBitmap(60, 40, frownBig, 16, 12, SH110X_WHITE);
    }
    display.display();

  }
  //camera setting sub menu
  if(m == 1 && p == 1){
    menuDisplayFormat("Camera Settings", 4, "Auto/Manual", "Rotation", "Change Camera ID", "Save", subM);
  }
  if(m == 1 && p == 2){
    //auto/manual sub sub menu
    if(s == 0){
      menuDisplayFormat("Auto/Manual", 2, "Auto Settings", "Manual Settings", "N/A", "N/A", subSubM); 
    }
    //rotation sub sub menu
    if(s == 1){
      menuDisplayFormat("Rotation", 4, "180 Degress", "Vertical", "Horizontal", "Reset", subSubM);
    }
    //change camera id sub menu, special function to change the id up or down
    if(s == 2){
      display.clearDisplay();
      updateConnectionStatus();
      display.setCursor(0,0);
      displayCamID(cameraID);
      display.setTextSize(1);
      display.setCursor(0,10);
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.print("Change Camera ID");
      display.setTextColor(SH110X_WHITE);
      display.setCursor(10,20);
      display.println("Press SEL to save");
      display.drawTriangle(59, 30, 54, 35, 64, 35, SH110X_WHITE); //triangle to signify up arrow
      display.setCursor(10,40);
      display.print("New ID: ");
      display.println(newcameraID);
      display.drawTriangle(54, 50, 64, 50, 59, 55, SH110X_WHITE); //triangle to signift down arrow
      display.display();
    }
  }
  if(m == 1 && s == 0 && p == 3){
    //auto sub sub sub menu
    if(sS == 0){ 
      menuDisplayFormat("Auto Settings", 3, "All Auto", "Auto Exposure","Auto WhiteBalance", "N/A", subSubSubM);
    }//manual sub sub sub menu
    else if(sS == 1){
      menuDisplayFormat("Manual Settings", 3, "All Manual", "Manual Exposure","Manual WhiteBalance", "N/A", subSubSubM);
    }

  }
  
}
  
//tentative start up screen that will display information after the splash logo
void startUpScreen(){ 
  display.setCursor(0,0);
  displayCamID(cameraID);
  display.setCursor(90,0);
  display.println(temperature);
  display.drawLine(0, 10, 128, 10, SH110X_WHITE);
  display.display();
}

void updateConnectionStatus(){
  display.drawRect(120, 0, 8, 6, SH110X_BLACK);
  if(connectionStatus == 0){
    display.drawBitmap(120, 0, smile, 8, 6, SH110X_WHITE);
  }else{
    display.drawBitmap(120, 0, frown, 8, 6, SH110X_WHITE);
  }
}

void checkCamConnectionStatus(){
    int prevCamID = cameraID;
    getCamID();
    if(cameraID != prevCamID){
      startUpCommands();
    }else{
      getConnectionStatus();
    } 
}

void updateCamID(int currentcamID, int newcamID){
    sendCommand(currentcamID, "rs485_addr", newcamID);
    getCamID();
    saveSettings();
}

int changeCamID(int cID, bool z){
  if(z){
    cID++;
    if(cID > 99){
      cID = 1; 
    }
  }else{
    cID--;
    if(cID < 1){
      cID = 99; 
    }
  }
  return cID;
}

void startUpCommands(){
  masterCommand(); //will receive identifyResponse and check connection command

  getConnectionStatus();

  getCamID(); //self explanatory

  newcameraID = cameraID; //initializes newcameraID to start off at whatever value the cameraID is at

  promptCommand(); //prompts to receive the "OK" at the end of succesful command
  
  menuLocation(mainM, subM, subSubM, subSubSubM, pos);  
  
  indicator(menuCursorLocation(mainM, subM, subSubM, subSubSubM, pos));
  
  updateConnectionStatus();

  getTemp(); //not working, but meant to measure temperature when connected to camera

}

String saveSettings(){
  String successful = "OK";
  String status;
  Serial1.flush();
  digitalWrite(2, HIGH);
  Serial1.print(cameraID);
  Serial1.print(" save_settings");
  Serial1.print("\r");
   while(!(USART1.STATUS & USART_TXCIF_bm)){
    //Serial.print("waiting");
  }
  digitalWrite(2, LOW);
  String x = Serial1.readString();
  x.trim();
  if( x.endsWith(successful)){
    status = ":)";
  }else{
    status = ":(";
  }
  return status;
}

//template to send commands to camera
char sendCommand(int cID, String comm, int v){ 
  int y=0;
  String cS = "OK";
  Serial1.flush();
  digitalWrite(2, HIGH); //write
  Serial1.print(cID);
  Serial1.print(" ");
  Serial1.print(comm);
  Serial1.print(" ");
  Serial1.print(v);
  Serial1.print("\r");
  while(!(USART1.STATUS & USART_TXCIF_bm)){
    //Serial.print("waiting");
  }
  digitalWrite(2, LOW); //read
  String x = Serial1.readString();
  x.trim();
  if( x.equals(cS)){
    connectionStatus = 0;
    return 0;
  }else{
    connectionStatus = 1;
    return 1;
  }
  
}

void displayCamID(int cID){ //displays camera at top left of oled screen
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("Camera: ");
  display.print(cID);
}

void promptCommand(){ //makes sure that when a command is received succesfully it sends an "OK"
  int y;
  digitalWrite(2, HIGH);
  Serial1.println(cameraID);
  Serial1.println(" prompt");
  Serial1.println("\r");
   while(!(USART1.STATUS & USART_TXCIF_bm)){
    //Serial.print("waiting");
  }
  digitalWrite(2, LOW); //read
}

void getConnectionStatus(){ 
  String cS = "OK";
  String response = masterCommand(); //takes Identify Response and checks if it ends with "OK", which means its collected
  response.trim();
  if( response.endsWith(cS)){
    connectionStatus = 0;
  }else{
    connectionStatus = 1;
  }
}
void getTemp(){
  Serial1.flush();
  digitalWrite(2, HIGH); //write
  Serial1.print(cameraID);
  Serial1.print(" ");
  Serial1.print("temp ");
  Serial1.print(0);
  Serial1.print("\r");
  while(!(USART1.STATUS & USART_TXCIF_bm)){
    //Serial.print("waiting");
  }
  digitalWrite(2, LOW); //read
  String x = Serial1.readStringUntil('K');
  x.trim();
  String temp = x.substring(x.indexOf(' ', x.indexOf(' ') + 1), x.indexOf(' ', x.indexOf(' ', x.indexOf(' ')+ 1) + 1));
  temp.trim();
  temperature = temp.toFloat();

}

void getCamID(){ //uses the mastercommand() to grab the camID 
  String camID;
  String response = masterCommand();

  response.trim();
  camID = response.substring(response.indexOf(' ', response.indexOf(' ') + 1), response.indexOf(' ', response.indexOf(' ', response.indexOf(' ')+ 1) + 1));
  camID.trim();
  cameraID = camID.toInt();
}

String masterCommand(){ //command used for camera start up that will gather general information about the camera, including the cam id
  String response;
  Serial1.flush();
  digitalWrite(2, HIGH);
  Serial1.print("100 identify");
  Serial1.print("\r");
  while(!(USART1.STATUS & USART_TXCIF_bm)){
    //Serial.print("waiting");
  }
  digitalWrite(2, LOW);
  response = Serial1.readStringUntil('K');
  response = response + "K";
  return response;
}

void indicator(int indic){
  //clears spaces where an indicator would go to refresh
  display.setCursor(0,20);
  display.println(" ");
  display.setCursor(0,30);
  display.println(" ");
  display.setCursor(0,40);
  display.println(" ");
  display.setCursor(0,50);
  display.println(" ");
  
  //8 is the value in the cursor indicator array that denotes no indicator needed
  if(indic != 8){
    display.setCursor(0, (indic+2)*10);
    display.println(">");
  };
  display.display();
}