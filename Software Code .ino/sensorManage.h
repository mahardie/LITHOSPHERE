//Laser comms setup

byte  incomingByte [13] = {};

//Start the sensor
void initSensor() {

      //Set serial2
      SERIAL_S.begin(19200, SERIAL_8N1); // coms to laser    
      delay(200);
      SERIAL_S.write(autobdr, sizeof(autobdr));
      delay(200);
      
      SERIAL_S.write(rdstatus, sizeof(rdstatus));
      delay(200);
      Serial.print(F("\nModule Latest Status : "));
      if(SERIAL_S.available() > 0){ // setup the RX-TX communication     
          SERIAL_S.readBytes(incomingByte, 13);
          for(int i = 0; i < 13; i++) {
            if(incomingByte[i] < 10)  Serial.print("0");
            Serial.print(incomingByte[i], HEX); 
            Serial.print(" ");
          }
          Serial.println();
      }
                  
      SERIAL_S.write(rdiv, sizeof(rdiv));
      delay(200);  
      Serial.print(F("\nInput Voltage : "));
      if(SERIAL_S.available() > 0){ // setup the RX-TX communication     
          SERIAL_S.readBytes(incomingByte, 13);
          for(int i = 0; i < 13; i++) {
            if(incomingByte[i] < 10)  Serial.print("0");
            Serial.print(incomingByte[i], HEX); 
            Serial.print(" ");
          }
          Serial.println();
      }


      SERIAL_S.write(rdhwn, sizeof(rdhwn));
      delay(200);
      Serial.print(F("\nModule HW Version : "));
      if(SERIAL_S.available() > 0){ // setup the RX-TX communication     
          SERIAL_S.readBytes(incomingByte, 13);
          for(int i = 0; i < 13; i++) {
            if(incomingByte[i] < 10)  Serial.print("0");
            Serial.print(incomingByte[i], HEX); 
            Serial.print(" ");
          }
          Serial.println();
      }


      SERIAL_S.write(rdswn, sizeof(rdswn));
      delay(200);
      Serial.print(F("\nModule SW Version : "));
      if(SERIAL_S.available() > 0){ // setup the RX-TX communication     
          SERIAL_S.readBytes(incomingByte, 13);
          for(int i = 0; i < 13; i++) {
            if(incomingByte[i] < 10)  Serial.print("0");
            Serial.print(incomingByte[i], HEX); 
            Serial.print(" ");
          }
          Serial.println();
      }

      SERIAL_S.write(rdsn, sizeof(rdsn));
      delay(200);
      
      display.setTextSize(1); display.setCursor(0, 45); display.println(F("Serial \nNumber : \r\n")); 
      
      if(SERIAL_S.available() > 0){ // setup the RX-TX communication     
          SERIAL_S.readBytes(incomingByte, 13);
          for(int i = 0; i < 13; i++) {
            if(incomingByte[i] < 10)  {
              display.print("0");
            }
            display.print(incomingByte[i], HEX); 
          }
          display.println();
      }

      display.setCursor(0, 120);
      display.print(F("BAT: "));
      display.print(batlevel);        
                
      display.display();   
}
