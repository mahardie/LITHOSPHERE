//format bytes

int write_msg_fail = -1;

String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  }else if (filename.endsWith(".txt")) {
    return "application/x-txt";
  }else if (filename.endsWith(".csv")) {
    return "application/x-csv";
  }
  
  return "text/plain";
}

bool exists(String path){
  bool yes = false;
  File file = FILESYSTEM.open(path, "r");
  if(!file.isDirectory()){
    yes = true;
  }
  file.close();
  return yes;
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) {    path += "index.html";  }
  
  String contentType = getContentType(path);
  
  if (exists(path)) {
    File file = FILESYSTEM.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void readFile(){
  
    Serial.println("*****************************************");

    file = FILESYSTEM.open("/data.txt");
    if(!file){
        Serial.println("- failed to open file for reading");
    }else{
          Serial.println("- read from file:");
          if(sysmode == 0) {
              while(file.available()){
                  Serial.write(file.read());
              }
          }
          else if(sysmode == 2) {
              while(file.available()){
                  //SerialBT.write(file.read());
                  delay(1);
              }            
          }
    }

    Serial.println("\r\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    file.close();
    delay(50);
}


void writeNewFile(char message[]){  

    if(FILESYSTEM.exists("/data.txt")) {      
      Serial.printf("Deleting file\r\n");      
      if(FILESYSTEM.remove("/data.txt"))         Serial.println("- file deleted");
      else                                       Serial.println("- delete failed");
    }
 
    Serial.printf("Writing New file: data.txt\r\n");        
    file = FILESYSTEM.open("/data.txt", FILE_WRITE);        
    if(!file){
      Serial.println("- failed to open file for writing");
    }
    else {
      if(file.print(message))  Serial.println("- file writtenx");
      else                     Serial.println("- write failedx");
    }
    
    file.close();    

    delay(50);
}

void appendFile(char message[]){   
  
  if(FILESYSTEM.exists("/data.txt")) {    
    file = FILESYSTEM.open("/data.txt", FILE_APPEND);    
    if(!file)                                     Serial.println("- failed to open file for appending");
    else {
      if(file.print(message))                     Serial.printf("- message appended to file: data.txt\r\n");
      else                                        Serial.printf("- append failed data.txt\r\n");
    }
    
    write_msg_fail = file.size();
    file.close();
    
  }
  else {
    
      Serial.println("No File");
      write_msg_fail = -1;
                
      //FILESYSTEM.begin();
      File root = FILESYSTEM.open("/");
      file = root.openNextFile();
      while(file){
          String fileName = file.name();
          size_t fileSize = file.size();
          Serial.printf("FS File: %s\n", fileName.c_str());
          file = root.openNextFile();
      }
      Serial.println(); 
  }
  
  delay(50);
}


void writeIndexFile(){  
  
    if(FILESYSTEM.exists("/index.html")) {      
      Serial.println("Deleting HTML file");      
      if(FILESYSTEM.remove("/index.html"))       Serial.println("- HTML file deleted");
      else                                       Serial.println("- HTML delete failed");
    }
            
    Serial.println("Writing file: index.html");        
    file = FILESYSTEM.open("/index.html", FILE_WRITE);        
    if(!file){
      Serial.println("- HTML failed to open file for writing");
    }
    else {
      if(file.print(HTMLText))  Serial.println("- HTML file written");
      else                      Serial.println("- HTML write failed");
    }
        
    file.close();    
    delay(50);
}


void readSettingFile(uint8_t type){
  
    Serial.println("*****************************************");

    file = FILESYSTEM.open("/settdata.txt");
    if(!file){
        Serial.println("- failed to open file for reading S");
    }else{
          Serial.print("- read from file S: ");
          memset(allsettings, '\0', sizeof(allsettings));
          settIndex = 0;
          while(file.available()) {
            allsettings[settIndex++] = file.read();
          }
          
          Serial.println(allsettings);
          sscanf(allsettings, "%d,%d,%d,%d,%lu,%d,%d,%d", &Ring_Diameter, &Ring_Insertion, &Soil_Type, &Depth_of_Water, &IR_Time_GAP, &Hole_diameter, &Depth_of_WaterB, &Hole_Depth);
    }

    Serial.println("\r\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    file.close();
    delay(50);
}

void writeSettingFile(char message[], uint8_t type){

    if(FILESYSTEM.exists("/settdata.txt")) {      
      Serial.printf("Deleting Setting file\r\n");      
      if(FILESYSTEM.remove("/settdata.txt"))         Serial.println("- file deletedq");
      else                                           Serial.println("- delete failedq");
    }

    delay(50); 
   
    Serial.printf("Writing file: settdata.txt\r\n");        
    file = FILESYSTEM.open("/settdata.txt", FILE_WRITE);        
    if(!file){
      Serial.println("- failed to open file for writing");
    }
    else {
      
      if(file.print(message))  Serial.println("- file written");
      else                     Serial.println("- write failed");
    }
    
    file.close();    
    delay(50);
}


void getRunNo() {

    Serial.println("*****************************************");

    file = FILESYSTEM.open("/rundata.txt");
    if(!file){
        Serial.println("- failed to open file for reading R");
    }else{
          Serial.print("- read from file R: ");
          memset(all_no, '\0', sizeof(all_no));
          rIndex = 0;
          while(file.available()) {
            all_no[rIndex++] = file.read();
          }
          
          Serial.println(all_no);
          sscanf(all_no, "%d", &run_no);
    }

    Serial.println("\r\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    file.close();
    delay(50);  
}

void setRunNo(char message[]) {

    if(FILESYSTEM.exists("/rundata.txt")) {      
      Serial.printf("Deleting Run Number file\r\n");      
      if(FILESYSTEM.remove("/rundata.txt"))          Serial.println("- file deleteda");
      else                                           Serial.println("- delete faileda");
    }

    delay(50); 
   
    Serial.printf("Writing file: rundata.txt\r\n");        
    file = FILESYSTEM.open("/rundata.txt", FILE_WRITE);        
    if(!file){
      Serial.println("- failed to open file for writing");
    }
    else {
      
      if(file.print(message))  Serial.println("- file written");
      else                     Serial.println("- write failed");
    }
    
    file.close();    
    delay(50);
}

void deleteFile(int re){
    
    writeNewFile("");
    setRunNo("0");

    if(re == DELETE_ALL_FILES) {
      //Ring_Diameter, Ring_Insertion, Soil_Type, Depth_of_Water, IR_Time_GAP, Hole_diameter, Depth_of_WaterB, Hole_Depth
      writeSettingFile("160,30,2,10,60000,100,100,100", ABOVE_GROUND);//MH edit the 3 was 1 for soil type
    }
    
    delay(50);
    
    if(re > DELETE_NO_RESTART) ESP.restart(); // restart the device immediately
}

bool deleteFileHTTP() {
    deleteFile(DELETE_NO_RESTART);
    appendFile(appenNewLine);
    server.send(200, F("text/plain"), F(" - File Deleted \r\n - New File Created \r\n - Device restarted \r\n - You may have to connect WiFi again!"));
    restartOnHttpDELETE = true;
    return true;
}
