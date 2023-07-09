#include "secrets.h"
#include "constants.h"
#include "mqttHelper.h"
#include "WifiHelper.h"
#include "DHT22.h"
#include "ArduinoJson.h"
#include "custom_logger.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

MqttHelper mqttHelper;
WifiHelper wifiHelper;
TemperatureDHT22 temperatureSensor;

float lastSentTemperature = 0;
float lastSentHumidity = 0;
float lastSentUpdate = 0;

long previous5MinMillis = 0;
long previous30SecMillis = 0;
long previous1HourMillis = 0;

void setup()
{ 
  Serial.begin(115200);

  CustomLogger::initialize();

  lastSentTemperature = 0;
  lastSentHumidity = 0;
  lastSentUpdate = 0;
  
  wifiHelper.connect();

  server.on("/get/logs", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(SPIFFS, LOG_PATH);
    String logs = CustomLogger::getLogs();  
    request->send(200, "text/plain", logs);
  });

  server.on("/clear/logs", HTTP_GET, [] (AsyncWebServerRequest *request) {
    CustomLogger::clearLogs();  
    request->send(200);
  });
  
  server.begin();
  
  temperatureSensor.initialize();

  mqttHelper.sendConfiguration();

  delay(1500);
}

void loop()
{
  unsigned long currentMillis = millis();

  if(currentMillis - previous5MinMillis >= 5 * INTERVAL_1_MIN_MILLIS) {
    CustomLogger::append("START SENDING TEMP");

    if(wifiHelper.isConnected()) {
      float temperature = temperatureSensor.getTemperature();
      float humidity = temperatureSensor.getHumidity();
  
      Serial.println("temperature: " + String(temperature));
      Serial.println("humidite: " + String(humidity));
  
      Serial.println(temperature - lastSentTemperature);
      if((temperature - lastSentTemperature >= MINIMUM_TEMPERATURE_UPDATE_DELTA || temperature - lastSentTemperature <= -1 * MINIMUM_TEMPERATURE_UPDATE_DELTA) && temperature >= MIN_TEMPERATURE_VALUE && temperature <= MAX_TEMPERATURE_VALUE ) {
        lastSentTemperature = temperature;
        bool didSend = mqttHelper.send(TEMPERATURE_STATE,String(temperature));
  
        if(didSend)
          lastSentUpdate = currentMillis;
        
      }
  
      Serial.println(humidity - lastSentHumidity);
      if((humidity - lastSentHumidity >= MINIMUM_HUMIDITY_UPDATE_DELTA || humidity - lastSentHumidity <= -1 * MINIMUM_HUMIDITY_UPDATE_DELTA) && humidity >= MIN_HUMIDITY_VALUE && humidity <= MAX_HUMIDITY_VALUE) {
        lastSentHumidity = humidity;
        bool didSend = mqttHelper.send(HUMIDITY_STATE,String(humidity));
  
        if(didSend) 
          lastSentUpdate = currentMillis;
        
      }
  
      mqttHelper.send(TEMPERATURE_AVAILABILITY, "online");
      mqttHelper.send(HUMIDITY_AVAILABILITY, "online");
    }

    previous5MinMillis = currentMillis;
    CustomLogger::append("END SENDING TEMP");
  }

  if(currentMillis - lastSentUpdate >= INTERVAL_1_HOUR_MILLIS) {
    CustomLogger::append("START RESTART");
    ESP.restart();
    lastSentUpdate = currentMillis;
    CustomLogger::append("END RESTART");
  }
  
  if (currentMillis - previous30SecMillis >= 30 * INTERVAL_1_SEC_MILLIS) {
    CustomLogger::append("START RECONNECT");

    wifiHelper.reconnectSequence();
    
    previous30SecMillis = currentMillis;
    CustomLogger::append("END RECONNECT");
  }

  if(currentMillis - previous1HourMillis >= INTERVAL_1_HOUR_MILLIS) {
    CustomLogger::append("START SEND CONFIGURATION");

    mqttHelper.sendConfiguration();
    
    previous1HourMillis = currentMillis;
    CustomLogger::append("END SEND CONFIGURATION");
  }
}
