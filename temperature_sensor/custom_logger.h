#ifndef CUSTOM_LOGGER_H
#define CUSTOM_LOGGER_H

#include "FS.h"
#include "SPIFFS.h"

const String LOG_PATH = "/log/debug_log.txt";

class CustomLogger {
  public:
    static void initialize();
    static void append(String record);
    static String getLogs();
    static void clearLogs();
};

void CustomLogger::initialize() {
  SPIFFS.begin();
}

void CustomLogger::clearLogs() {
  if(SPIFFS.remove(LOG_PATH)){
      Serial.println("- file deleted");
  } else {
      Serial.println("- delete failed");
  }
}

void CustomLogger::append(String record) {
  File file = SPIFFS.open(LOG_PATH, FILE_APPEND);
  if(!file){
      Serial.println("- failed to open file for appending");
      return;
  }

  String record_with_time = String(millis()) + "-" + record + "\n";
  
  if(file.print(record_with_time)){
      Serial.println("- message appended");
  } else {
      Serial.println("- append failed");
  }
}

String CustomLogger::getLogs() {
  File file = SPIFFS.open(LOG_PATH);
  if(!file || file.isDirectory()){
      Serial.println("- failed to open file for reading");
      return "";
  }

  Serial.println("- read from file:");
  String logs = "";
  while(file.available()){
      logs += (char) file.read();
  }

  return logs;
}

#endif
