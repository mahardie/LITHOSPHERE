
#define IR_Time_STEP         60000 //mh changed from 5000
#define Ring_Diameter_STEP   10
#define Ring_Insertion_STEP  5// mh changed from 10
#define Depth_of_Water_STEP  1
#define input_WAIT_STEP      3000

#define Hole_diameter_STEP   1
#define Hole_Depth_STEP      10
#define Depth_of_WaterB_STEP 1


void IRAM_ATTR captureUp() {

  if      (inputMode == 0 && Ring_Diameter  + Ring_Diameter_STEP  <= 300)   {         Ring_Diameter  += Ring_Diameter_STEP;     }
  else if (inputMode == 1 && Ring_Insertion + Ring_Insertion_STEP <= 100)   {         Ring_Insertion += Ring_Insertion_STEP;    }
  else if (inputMode == 2)                                                  {         Soil_Type = (Soil_Type + 1) % 4;          }
  else if (inputMode == 3 && Depth_of_Water + Depth_of_Water_STEP <= 50)    {         Depth_of_Water += Depth_of_Water_STEP;    }  
  else if (inputMode == 4 && IR_Time_GAP    + IR_Time_STEP <= 600000UL)     {         IR_Time_GAP    += IR_Time_STEP;           }  //MH changed time step limit to 10 mins

  inputsTimer = millis();
}

void IRAM_ATTR captureDown() {

  if      (inputMode == 0 && Ring_Diameter  - Ring_Diameter_STEP  >= 50)   {         Ring_Diameter  -= Ring_Diameter_STEP;     }
  else if (inputMode == 1 && Ring_Insertion - Ring_Insertion_STEP >= 10)   {         Ring_Insertion -= Ring_Insertion_STEP;    }
  else if (inputMode == 2)                                                 {         }
  else if (inputMode == 3 && Depth_of_Water - Depth_of_Water_STEP >= 5)   {         Depth_of_Water -= Depth_of_Water_STEP;    }  //mh
  else if (inputMode == 4 && IR_Time_GAP    - IR_Time_STEP >= 60000)        {         IR_Time_GAP    -= IR_Time_STEP;           }  
  
  inputsTimer = millis();
}

void IRAM_ATTR captureUpbelow() {

  if      (inputMode == 0 && Hole_diameter   + Hole_diameter_STEP  <= 1000)  {         Hole_diameter   += Hole_diameter_STEP;    }
  else if (inputMode == 1 && Hole_Depth      + Hole_Depth_STEP <= 1000)      {         Hole_Depth      += Hole_Depth_STEP;       }
  else if (inputMode == 2 && Depth_of_WaterB + Depth_of_WaterB_STEP <= 1000) {         Depth_of_WaterB += Depth_of_WaterB_STEP;  }  
  else if (inputMode == 3 && IR_Time_GAP     + IR_Time_STEP <= 6000000)         {         IR_Time_GAP     += IR_Time_STEP;          }  //mh edit

  inputsTimer = millis();
}

void IRAM_ATTR captureDownbelow() {

  if      (inputMode == 0 && Hole_diameter   - Hole_diameter_STEP  >= 30)   {         Hole_diameter   -= Hole_diameter_STEP;     }
  else if (inputMode == 1 && Hole_Depth      - Hole_Depth_STEP >= 10)        {         Hole_Depth      -= Hole_Depth_STEP;        }
  else if (inputMode == 2 && Depth_of_WaterB - Depth_of_WaterB_STEP >= 10)  {         Depth_of_WaterB -= Depth_of_WaterB_STEP;   }  
  else if (inputMode == 3 && IR_Time_GAP     - IR_Time_STEP >= 60000)         {         IR_Time_GAP     -= IR_Time_STEP;           }  
  
  inputsTimer = millis();
}

void IRAM_ATTR captureNext() {
  inputMode++;
  inputsTimer = millis();
}

//---------------------------------------------------------------------------------------------------------------------------------

void IRAM_ATTR captureDelFile() {
    deleteFileLevel = DELETE_ONLY_DATA;
    //deleteFile(DELETE_ONLY_DATA);
}


void IRAM_ATTR captureDelAll() {
    deleteFileLevel = DELETE_ALL_FILES;
    //deleteFile(DELETE_ALL_FILES);
}

//---------------------------------------------------------------------------------------------------------------------------------

void takeALLinputs(uint8_t type) {

  if(type == ABOVE_GROUND) {
    
            inputMode = 0;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 0) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F("   Ring \r\n Diameter"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print(" ");
              display.println(Ring_Diameter);
              display.println(" mm");
              display.display();
            }
          
            inputMode = 1;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 1) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F("   Ring \r\nInsertion"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print(" ");
              display.println(Ring_Insertion);
              display.println(" mm");
              display.display();
            }
          
            inputMode = 2;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 2) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(5, 10); 
              display.println(F("Soil Type"));
              display.setTextSize(1); display.setCursor(0, 30); 
              
                   if(Soil_Type == 0)  display.println(F("Compact Clay & Silt \r\n\r\nAlpha = \r\n"));
              else if(Soil_Type == 1)  display.println(F("Unstruct. Clay-Sand \r\n\r\nAlpha = \r\n"));
              else if(Soil_Type == 2)  display.println(F("Struct. Clays-Loams \r\n\r\nAlpha = \r\n"));
              else if(Soil_Type == 3)  display.println(F("Extremely Struct. Soil & Gravels \r\n\r\nAlpha = \r\n"));
          
              display.setTextSize(2);
              display.println(Alpha[Soil_Type], 3);
              display.display();
            }      
          
            inputMode = 3;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 3) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F("   Water \r\n   Depth"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print("  ");
              display.println(Depth_of_Water);
              display.println("  mm");
              display.display();
            } 

            inputMode = 4;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 4) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F(" IR Time \r\n   Gap"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print("  ");
              display.println(IR_Time_GAP/ 60000); //mh change 1000 to 60000
              display.println("  min");//mh change
              display.display();
            }

  }
  else if(type == BELOW_GROUND)  {
                                  
            inputMode = 0;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 0) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F("   Hole \r\n Diameter"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print(" ");
              display.println(Hole_diameter);
              display.println(" mm");
              display.display();
            }
          /* MH edit remove hole depth at startup
            inputMode = 1;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 1) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F("   Hole\r\n  Depth"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print(" ");
             display.println(Hole_Depth);// mh
              display.println(" mm");
              display.display();
            }
          */
            inputMode = 2;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 2) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F("   Water \r\n Depth(B)"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print("  ");
              display.println(Depth_of_WaterB);
              display.println("  mm");
              display.display();
            } 
          
            inputMode = 3;
            inputsTimer = millis();
            while(millis() - inputsTimer < input_WAIT_STEP && inputMode == 3) {
              display.clearDisplay();
              display.setTextSize(1); display.setCursor(0, 10); 
              display.println(F(" IR Time \r\n   Gap"));
              display.setTextSize(2); display.setCursor(0, 50); 
              display.print("  ");
              display.println(IR_Time_GAP / 60000); //mh change
              display.println("  min"); //mh change
              display.display();
            }        
  }
}

//
//void IRAM_ATTR captureDayMonYea() {
//
//  if      (inputMode == 0 && Ring_Diameter  + 10 <= 300)  {         Ring_Diameter  += 10; }
//  else if (inputMode == 1 && Ring_Insertion + 10 <= 100)  {         Ring_Insertion += 10; }
//  else if (inputMode == 2)                                {         Soil_Type = (Soil_Type + 1) % 4;  }
//  else if (inputMode == 3 && Depth_of_Water +  1 <= 50)   {         Depth_of_Water++;     }  
//
//  inputsTimer = millis();
//}
//
//
//void IRAM_ATTR captureDNext() {
//
//  if      (inputMode == 0 && Ring_Diameter  + 10 <= 300)  {         day_of_month  += 10; }
//  else if (inputMode == 1 && Ring_Insertion + 10 <= 100)  {         month += 10; }
//  else if (inputMode == 2)                                {         year += (Soil_Type + 1) % 4;  }
//
//  inputsTimer = millis();
//}
