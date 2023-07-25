/*********
Ananda Maiti
ICT/UTAS
2021 Sept-Oct
Verion May 11 2022
Verion Jun 25 2022

Marcus Hardie
TIA/UTAS
Designed for Adafruit ESP32 Feather with Adafruit OLED 64 
*********/
/* Wiring guide// MH
Laser
  Red - Power -3.3vt
  Yellow - PWREN- 3.3vt
  Black - Ground 
  White RX goes to TX (15)
  Blue TX goes to RX (16)
Switch
  Blk - grd
  Red - EN
Battery 3.7 Vt
  Red - Bat
  Black- GRD
*/


//Hexadezimale-Codes from Usermanual
byte autobdr[] =  {0x55};//Auto Baud Rate
byte rdstatus[] = {0xAA, 0x80, 0x00, 0x00, 0x80}; //Read Module Latest Status
byte rdhwn[] =    {0xAA, 0x80, 0x00, 0x0A, 0x8A}; //Read Hardware Version Number
byte rdswn[] =    {0xAA, 0x80, 0x00, 0x0C, 0x8C}; //Read Software Version Number
byte rdsn [] =    {0xAA, 0x80, 0x00, 0x0E, 0x8E}; //Read Module Serial Number
byte rdiv[] =     {0xAA, 0x80, 0x00, 0x06, 0x86}; //Read Input Voltage
byte rdmres[] =   {0xAA, 0x80, 0x00, 0x22, 0xA2}; //Read Measure Result (usermanual S13 result)
byte lon[] =      {0xAA, 0x00, 0x01, 0xBE, 0x00, 0x01, 0x00, 0x01, 0xC1};// Laser on
byte loff[] =     {0xAA, 0x00, 0x01, 0xBE, 0x00, 0x01, 0x00, 0x00, 0xC0};//Laser off
byte shotauto[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x21};//Start 1-shot Auto Distance Measure
byte shotslow[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x01, 0x22};// Start 1-shot Slow Distance Measure
byte shotfast[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x02, 0x23};//Start 1-shot Fast Distance Measure
byte cntinusauto[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x04, 0x25}; //Start Continuous Auto Distance Measure auto
byte cntinusslow[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x05, 0x26};// Start Continuous Slow Distance Measure slow
byte cntinusfast[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x06, 0x27};// Start Continuous Fast Distance Measure fast
byte stopmes[] =     {0x58};//Stop continuous Mesure




//XXXXXXXXXXXXXXXhere Goes itXXXXXXXXXXXXXXX

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//#include "BluetoothSerial.h"
#include "FS.h"
#include <SPIFFS.h>

#include <WiFi.h>
#include <WebServer.h>
#include "basic.h"
#include "fileManage.h"

int   x = 0, count = 0, data_index = 0, shortmode = 0, val = 0, val_old = 0, ia = 0;
char  data2write[250], c;
bool  sentReq = false;
float batlevel = 0.0;

//All timers
unsigned long myTime, checkTimeOut, lastcheckTime, IRTimer, deleteTimer;

// Declaration for an SH1107 display connected to I2C (SDA, SCL pins)
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire); // mh edit

//Include Screen, WiFi, Buttons and Sensor Related functions (these must go just before the setup)
#include "displayManage.h"
#include "sensorManage.h"
#include "inputButtonManage.h"
#include "wifi_Manage.h"


void setup() {
    
      pinMode(BUTTON_A, INPUT_PULLUP);
      pinMode(BUTTON_B, INPUT_PULLUP);
      pinMode(BUTTON_C, INPUT_PULLUP);

      // check battery levels ASAP
      pinMode(BATPIN, INPUT);
      batlevel = (analogRead(BATPIN) / 4095.0) * 2.0 * 3.3 * 1.1;
      
      delay(250); // wait

      // setup the OLED
      display.begin(0x3C, true); // Address 0x3C default      
      display.setRotation(0);

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);//mh edit
      display.println(F("Starting \r\nin Mode: ")); 

      delay(250);

      //default is sysmode = 0 USB above ground
      if(     digitalRead(BUTTON_B) == LOW) { sysmode = 3; } // USB below ground
      else if(digitalRead(BUTTON_C) == LOW) { sysmode = 1; } // Wifi Mode
      else if(digitalRead(BUTTON_A) == LOW) { sysmode = 2; } // BT

      //Defualt is USB above ground, sysmode  => 0, sensor reading
      if(     sysmode == 1) display.print(F("\r\n- WiFi"));
      else if(sysmode == 0) display.print(F("\r\n- USB\r\n- Sensor\r\n- Abv. Gnd"));
      else if(sysmode == 3) display.print(F("\r\n- USB\r\n- Sensor\r\n- Blw. Gnd"));
      else if(sysmode == 2) display.print(F("\r\n- BTH\r\n- Sensor"));      
      display.display();


      // Start the Serial USB
      Serial.begin(19200);// comms to serial screen
      delay(250);


      //Start the FileSystem
      FILESYSTEM.begin(true);
      File root = FILESYSTEM.open("/");
      file = root.openNextFile();
      while(file) file = root.openNextFile();
      Serial.println(); 

      memset(HTMLText, '\0', sizeof(HTMLText));
      
      if(FILESYSTEM.exists("/data.txt")) {    
          // meaning this device is already loaded at least once
          file = FILESYSTEM.open("/data.txt");    
          if(!file) {                       
              Serial.println("- failed to open file for size");              
              sprintf(HTMLText, "<html><head><title>%s</title></head><body><br><br><center>%s<br>Files exists, but cannot open. Please try delete!</center><br></body></html>", ssid, ssid);                                                                           
              writeIndexFile();  
          } 
          else {
              size_of_file = file.size();
              sprintf(HTMLText, "<html><head><title>%s</title></head><body><br><br><br><br><br><center>%s<br><br><br><form action='/data.txt'><input type='submit' value='Download' /></form><br>File Size: %d.%03d KB<br><br><br><form action='/delete'><input type='submit' value='Delete' /></form></center></body></html>", ssid, ssid, (size_of_file / 1000), (size_of_file % 1000));                                                                           
              writeIndexFile();                
          }
          file.close();             
      }
      else {
          // meaning this device is being loaded first time
          
          deleteFile(DELETE_ALL_FILES);
          //just attempted to create the new files - data and settings, as if pressing the delete button
          //Device should restart now, so these code should never execute, but ...
          
          file = FILESYSTEM.open("/data.txt");    
          if(!file)  {                                   
              Serial.println("- failed to open file for size");
              sprintf(HTMLText, "<html><head><title>%s</title></head><body><br><br><center>%s<br>File not found/created, Reboot!</center><br></body></html>", ssid, ssid);                                                                           
              writeIndexFile();    
          }           
          else {
              size_of_file = file.size();
              sprintf(HTMLText, "<html><head><title>%s</title></head><body><br><br><center>%s<br>Files just created, No data!</center><br></body></html>", ssid, ssid);                                                                           
              writeIndexFile();   
          }
          file.close();  
      }
      
      delay(250);
     
      //---------------------------------------------------------------------------------------------

      if(sysmode == 0 || sysmode == 3) {
                                        //Inputs
                                        if(sysmode == 0) {
                                                            readSettingFile(ABOVE_GROUND);
                                                      
                                                            attachInterrupt(BUTTON_B, captureUp,   FALLING);
                                                            attachInterrupt(BUTTON_C, captureDown, FALLING);
                                                            attachInterrupt(BUTTON_A, captureNext, FALLING);
                    
                                                            takeALLinputs(ABOVE_GROUND);
                    
                                                            memset(allsettings, '\0', sizeof(allsettings));
                                                            sprintf(allsettings, "%d,%d,%d,%d,%lu,%d,%d,%d", Ring_Diameter, Ring_Insertion, Soil_Type, Depth_of_Water, IR_Time_GAP, Hole_diameter, Depth_of_WaterB, Hole_Depth);
                                                            Serial.println(allsettings);
                                                            writeSettingFile(allsettings, ABOVE_GROUND);
                                        }
                                        else if(sysmode == 3) {
                                          
                                                            readSettingFile(BELOW_GROUND);
                                                      
                                                            attachInterrupt(BUTTON_B, captureUpbelow,   FALLING);
                                                            attachInterrupt(BUTTON_C, captureDownbelow, FALLING);
                                                            attachInterrupt(BUTTON_A, captureNext,      FALLING);
                    
                                                            takeALLinputs(BELOW_GROUND);
                    
                                                            memset(allsettings, '\0', sizeof(allsettings));
                                                            sprintf(allsettings, "%d,%d,%d,%d,%lu,%d,%d,%d", Ring_Diameter, Ring_Insertion, Soil_Type, Depth_of_Water, IR_Time_GAP, Hole_diameter, Depth_of_WaterB, Hole_Depth);
                                                            Serial.println(allsettings);
                                                            writeSettingFile(allsettings, BELOW_GROUND);                                  
                                        }

                                        detachInterrupt(BUTTON_B); //, captureUp,   FALLING);
                                        detachInterrupt(BUTTON_C); //, captureDown, FALLING);
                                        detachInterrupt(BUTTON_A); //, captureNext, FALLING);
                                        
                                        noInterrupts();
                                  
                                        display.clearDisplay();
                                        display.setTextSize(1); display.setCursor(0, 0); 
                                        display.println(F("All Saved\r\n\r\nStarting\r\nSensor ..."));
                                        display.display();
                                        
                                        delay(200);
                                                            
                                        //---------------------------------------------------------------------------------------------
                                  
                                        // Getting the Run #
                                        getRunNo();  
                                        run_no++;
                                        memset(all_no, '\0', sizeof(all_no));
                                        sprintf(all_no, "%d", run_no);
                                        setRunNo(all_no); 
                                        
                                        memset(plusLine, '\0', sizeof(plusLine));
                                        sprintf(plusLine, "RUN %d [ %s ]", run_no, allsettings);      
                                  
                                        appendFile(appenNewLine);
                                        appendFile(appenNewLine);
                                        appendFile(plusLine);
                                        appendFile(appenNewLine);
                                        appendFile(appenNewLine);
                                  
                                        delay(200);
                                  
                                        //---------------------------------------------------------------------------------------------
                                        
                                        //Init the sensor
                                        initSensor();
                                        delay(3000);


                                        SERIAL_S.write(shotfast, sizeof(shotfast));// set type of laser measurement
                                        checkTimeOut = lastcheckTime = millis();  
                                        Serial.print(F("Time, Distance "));                                            
      }
      else if(sysmode == 2) {                                        
//                                        SerialBT.begin(ssid); //Bluetooth device name
                                        display.clearDisplay();
                                        display.setTextSize(1);
                                        display.print(F("BT Mode is not avialable"));
                                        display.display();        
      }      
      else if(sysmode == 1) {
        
          batlevel = (analogRead(BATPIN) / 4095.0) * 2.0 * 3.3 * 1.1;
  
          if(batlevel >= 3.7) {                     
                          startWiFi();
          }
          else {
                          notifyWiFiProblem();       
                          while (1) { delay(10000); }
          }
      }


      //set all timers
      myTime = checkTimeOut = lastcheckTime = IRTimer = millis();
}

void loop() {

  if(sysmode == 1) { //Wifi Mode

      if(restartOnHttpDELETE) ESP.restart();
      
      server.handleClient();
      delay(2);//allow the cpu to switch to other tasks
     
  }
  else if(sysmode == 0  || sysmode == 3 || sysmode == 2) {           

            batlevel = (analogRead(BATPIN) / 4095.0) * 2.0 * 3.3 * 1.1;
            
            if(millis() - checkTimeOut > RESET_REQ_TIME) { // If a response has not been received
                sentReq = false;                           // then prepare to ask the sensor again
            }
            
            if(millis() - myTime > 1000) {                // Update the screen every second
                myTime = millis();                              
                setRegularScreen();                
            }  

            //send a request for new data to sensor after SENSOR_TIME_GAP = 3000
            if(!sentReq && millis() - lastcheckTime > SENSOR_TIME_GAP) {
                sentReq = true;
                checkTimeOut = lastcheckTime = millis();  
                SERIAL_S.write(shotfast, sizeof(shotfast));// set type of laser measurement
                delay(100);         
            }
        
            if(SERIAL_S.available() > 0) { // setup the RX-TX communication with the sensor

                
                SERIAL_S.readBytes(incomingByte, 13);
                val = incomingByte[8];
                val = val << 8;
                val = val + incomingByte[9]; 

                  //code to set data limits - MH                 
                if (val < 20 || val > 10000) {
                val = val_old;
                  }
                else {
                 val_old=val;
                 }
                 
                // Serial Monitor output ...
                Serial.print(myTime/1000);
                Serial.print(", ");
                Serial.print(val, DEC);
                Serial.print(", ");
                Serial.print(IR, 2);
                Serial.print(", ");
                Serial.print(HC, 2);
                Serial.print(", ");
                Serial.print(avgDepth, 2);
                Serial.print(", ");

                if(sysmode == 0 && millis() - IRTimer > IR_Time_GAP) { // Above ground claculations
                
                        //change in depth over time, calculate the IR/HC every IR_TIME_GAP s   
                        avgDepth = 0;
                        for(ia = 0; ia < oldDepthIndex; ia++)
                            avgDepth += old_depths[ia];
    
                        avgDepth /= oldDepthIndex;
                            
                        IR = (1497.0 * 3600 * ((avgDepth - avgDepth_old) / ((millis() - IRTimer) / 1000.0))) / (3.1416 * ( (Ring_Diameter / 2.0) * (Ring_Diameter / 2.0)));// Surface Infiltration rate calculation
                        HC = IR / ((Depth_of_Water / ((( 0.9927 * Ring_Insertion) + (0.5781 * (Ring_Diameter / 2.0))))) + (1.0 / (Alpha[Soil_Type] * ((0.9927 * Ring_Insertion) + (0.5781 * (Ring_Diameter / 2.0))))) + 1.0);//Surface Hydraulic conductivity based on reynolds
                        
                        // Stamp the time when we got the last values
                        IRTimer = millis();
                        oldDepthIndex = 0;
                        avgDepth_old = avgDepth;
                }
                else if(sysmode == 3 && millis() - IRTimer > IR_Time_GAP) {  // Below ground claculations

                        //change in depth over time, calculate the IR/HC every IR_TIME_GAP s    
                        avgDepth = 0;
                        for(ia = 0; ia < oldDepthIndex; ia++)
                            avgDepth += old_depths[ia];
    
                        avgDepth /= oldDepthIndex;

                        IR =  (1497*((avgDepth - avgDepth_old)/(((millis() - IRTimer) / 1000.0)/3600)))/((3.142*(Hole_diameter/2))*((Hole_diameter/2)+(2*Depth_of_WaterB)));// new calculation
                        
                        HC = 1.6 *3600*((((avgDepth - avgDepth_old)/((millis() - IRTimer) / 1000.0)*1497)*(asinh(((Depth_of_WaterB)/(Hole_diameter/2))))-1)/(2*3.142*Depth_of_WaterB*Depth_of_WaterB));// brown book checked ok
                        
                       //HC =  3600.0 * (4.4 * (1497 *((avgDepth - avgDepth_old) / ((millis() - IRTimer) / 1000.0)/3600)) * ((0.5 * asinh(Depth_of_WaterB / (2 * (Hole_diameter * 5 / 2)))) - (sqrt((((Hole_diameter / 2) / Depth_of_WaterB) * (( Hole_diameter / 2) / Depth_of_WaterB)) + 0.25) + (( Hole_diameter / 2) / Depth_of_WaterB)))) / (2 * 3.1416 * Depth_of_WaterB * Depth_of_WaterB); // subsurface hydraulic conductivity aust standard.
 
                        // Stamp the time when we got the last values
                        IRTimer = millis();
                        oldDepthIndex = 0;
                        avgDepth_old = avgDepth;           
                }

                // Serial Monitor output ...
                Serial.println(avgDepth, 2);  
                
                //write to SPIFF
                memset(data2write, '\0', sizeof(data2write));
                sprintf(data2write, "%d, %d, %d, %d.%02d, %d.%02d, %d\r\n", (myTime / 1000), val, (int) (batlevel * 100), (int) IR, ((int) (IR * 100)) % 100, (int) HC, ((int) (HC * 100)) % 100, sysmode);
                appendFile(data2write);
                  
                // Display  text
                setRegularScreen();     
                           
                //showDataScreen();  
                display.display();              
                delay(100);

                //Transfer OLD values
                old_depths[oldDepthIndex++] = val;
            }
        
            if(digitalRead(BUTTON_A) == LOW) {
                displayUSBDownload();
                readFile();
                display.clearDisplay();
            }
            else if(digitalRead(BUTTON_B) == LOW) {

                  Serial.println("Entering Delete Mode");
                  while(digitalRead(BUTTON_B) == LOW);
                  
                  deleteFileLevel = -1;
                  Serial.println("A Entering Delete Mode");
                  attachInterrupt(BUTTON_B, captureDelFile, FALLING);
                  attachInterrupt(BUTTON_C, captureDelAll,  FALLING);

                  interrupts();
                  displayDELmessage();
                  deleteTimer = millis();
                  Serial.println("W Delete Mode");
                  
                  while(millis() - deleteTimer < 5000) {
                    delay(10);
                    Serial.print(".");
                    if(deleteFileLevel > 0) {
                      deleteFile(deleteFileLevel);
                      break;
                    }                    
                  }

                  
                  detachInterrupt(BUTTON_B);
                  detachInterrupt(BUTTON_C);
                  deleteFileLevel = -1;
                  noInterrupts();
            }
//            else if(digitalRead(BUTTON_C) == LOW) {
//                interrupts();
//
//
//
//
//                noInterrupts();
//            }
  }
}
