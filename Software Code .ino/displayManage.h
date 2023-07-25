void setDataScreen() {

    display.setTextSize(1);
    display.setCursor(0, 110);
    display.print(F("F:"));
    if(write_msg_fail < 0 )     {display.print(F(" Failed"));}
    else                        {display.print(write_msg_fail / 1024.0, 2); display.print("KB");}// F("Write \nSuccess"));
}
  
  
void setRegularScreen() {
  
    // Display  text
    display.clearDisplay();
    display.setTextSize(1);
//    display.setCursor(0, 0);
//    display.println(F("WiFi OFF"));
  
    display.setCursor(0, 0);

    display.print("T:");
    display.print((myTime / 1000));
    display.println(F("s"));
    
    display.setCursor(0, 15);                
    display.setTextSize(1); display.print(F("D (mm)\r\n "));
    display.setCursor(0, 27); 
    display.setTextSize(2); display.print(val, DEC);
  
    display.setCursor(0, 52);  
    display.setTextSize(1); display.print(F("IR (mm/hr)\r\n"));
    display.setCursor(0, 63); 
    display.setTextSize(1); display.print(IR, 2);
  
    display.setCursor(0, 80);  
    display.setTextSize(1); display.print(F("HC (mm/hr)\r\n"));
    display.setCursor(0, 90); 
    display.setTextSize(1); display.print(HC, 2);

    // File size goes at 0,100

    //Then the USB and BAT level
    display.setCursor(0, 120);
    display.setTextSize(1); 
    if(sysmode == 2)                        {display.print(F("B#")); display.print(all_no);}
    else if(sysmode == 0 || sysmode == 3)   {display.print(F("U#")); display.print(all_no);}
    
    //display.setCursor(0, 120);
    display.print(F(" B:"));
    display.println(batlevel);

    //display.display();
    setDataScreen();
}

void displayUSBDownload() {
  
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print(F("USB/BlueTooth DOWNLOAD"));
    display.display(); 
}

void displayDELmessage() {
  
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(F("DELETE ?\r\n"));
    display.println(F("C - ALL"));
    display.println(F("B - Only\r\n    Data"));
        
    display.display(); 
}
