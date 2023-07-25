void startWiFi() {                                                  
    WiFi.softAP(ssid, password);                                                                    
    IPAddress IP = WiFi.softAPIP();                                                                    
  
    //SERVER INIT: called when the url is not defined here
    //use it to load content from FILESYSTEM
    server.onNotFound([]() {
      if (!handleFileRead(server.uri())) {
        server.send(404, F("text/plain"), F("File Not Found"));
      }
    });
    
    server.on("/delete", HTTP_GET, deleteFileHTTP);
    
    server.begin();
    Serial.println(F("HTTP server started"));    

    //setup OLED
    display.clearDisplay();  
    display.setTextSize(1);          
    
    display.setCursor(0, 0);
    display.println(F("WiFi ON"));
    display.setCursor(0, 15);
    display.println(F("SSID: "));
    display.setCursor(0, 25);
    display.println(ssid);
    
    display.setCursor(0, 45);
    display.print(F("\r\n\r\nConnect & Open : \r\n\r\nhttp://")); //192.168.4.1
    display.print(IP);
    display.print(F("/\r\n\r\nin browser"));   

    display.setCursor(0, 120);
    display.print(F("BAT: "));
    display.print(batlevel);        
                    
    display.display();  
}

void notifyWiFiProblem() {
  
    display.clearDisplay();  
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println(F("Looks like you are out of power!"));
  
    display.setCursor(0, 120);
    display.print(F("BAT: "));
    display.print(batlevel);        
                    
    display.display(); 
}
