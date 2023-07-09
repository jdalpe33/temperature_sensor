#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ArduinoMqttClient.h>
#include "custom_logger.h"

const char* TAG = "MqttHelper";


class MqttHelper {
  private:
    WiFiClient wifiClient;
    MqttClient* mqttClient;

    bool connect();
    
  public:
    MqttHelper();
    bool send(String topic, String message);
    void sendConfiguration();
};

MqttHelper::MqttHelper() {
  mqttClient = new MqttClient(wifiClient);
}

bool MqttHelper::connect() {
  mqttClient->setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);

  Serial.print("Attempting to connect to the MQTT broker: ");
  CustomLogger::append("Attempting to connect to the MQTT broker: ");
  Serial.println(MQTT_HOST);

  int tries = 0;
  bool didConnect = false;
  do {
    didConnect = mqttClient->connect(MQTT_HOST, MQTT_PORT);
    tries++;
    delay(1000);
  } while(!didConnect && tries < 10);


  if(didConnect) {
    CustomLogger::append("You're connected to the MQTT broker!");
    Serial.println("You're connected to the MQTT broker!");
    Serial.println();
    return true;
  } else {
    CustomLogger::append("NOT connected to the MQTT broker!");
    return false;
  }
}

void MqttHelper::sendConfiguration() {
  StaticJsonDocument<512> hConfig;
  hConfig["device_class"] = "humidity";
  hConfig["unit_of_measurement"] = "%";
  hConfig["state_class"] = "measurement";
  hConfig["name"] = "Taux humidité salon";
  hConfig["state_topic"] = HUMIDITY_STATE;
  hConfig["availability_topic"] = HUMIDITY_AVAILABILITY;
  hConfig["unique_id"] = "cb5bf11add82";
  JsonObject hDeviceObject = hConfig.createNestedObject("device");
  hDeviceObject["identifiers"] = "living_room_sensor";
  hDeviceObject["name"] = "Capteur salon";
  hDeviceObject["sw_version"] = "1.0.0";
  hDeviceObject["model"] = "esp32dev";
  hDeviceObject["manufacturer"] = "espressif";
  String hConfigString;
  serializeJson(hConfig, hConfigString);

  StaticJsonDocument<512> tConfig;
  tConfig["device_class"] = "temperature";
  tConfig["unit_of_measurement"] = "°C";
  tConfig["state_class"] = "measurement";
  tConfig["name"] = "Temperature salon";
  tConfig["state_topic"] = TEMPERATURE_STATE;
  tConfig["availability_topic"] = TEMPERATURE_AVAILABILITY;
  tConfig["unique_id"] = "0242ac130002";
  JsonObject tDeviceObject = tConfig.createNestedObject("device");
  tDeviceObject["identifiers"] = "living_room_sensor";
  tDeviceObject["name"] = "Capteur salon";
  tDeviceObject["sw_version"] = "1.0.0";
  tDeviceObject["model"] = "esp32dev";
  tDeviceObject["manufacturer"] = "espressif";
  String tConfigString;
  serializeJson(tConfig, tConfigString);

  send(HUMIDITY_CONFIG, hConfigString);
  send(TEMPERATURE_CONFIG, tConfigString);
}

bool MqttHelper::send(String topic, String message) {
  bool didConnect = connect();

  if(didConnect) {
    CustomLogger::append("Sending message to topic: " + topic + "," + message);
    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println("");
    Serial.println(message);
  
    // send message, the Print interface can be used to set the message contents
    mqttClient->beginMessage(topic, message.length(), false, 0, false);
    mqttClient->print(message);
    mqttClient->endMessage();

    return true;
  } else {
    return false;
  }
}

#endif
