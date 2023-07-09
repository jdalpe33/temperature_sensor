#ifndef CONSTANTS_H
#define CONSTANTS_H

const int MQTT_PORT = 1883;
const char* MQTT_HOST = "192.168.0.117";

const char* TEMPERATURE_CONFIG = "homeassistant/sensor/livingRoomTemperature/config";
const char* HUMIDITY_CONFIG = "homeassistant/sensor/livingRoomHumidity/config";
const char* TEMPERATURE_STATE = "homeassistant/sensor/livingRoomTemperature/state";
const char* HUMIDITY_STATE = "homeassistant/sensor/livingRoomHumidity/state";
const char* TEMPERATURE_AVAILABILITY = "homeassistant/sensor/livingRoomTemperature/availability";
const char* HUMIDITY_AVAILABILITY = "homeassistant/sensor/livingRoomHumidity/availability";

const float MINIMUM_TEMPERATURE_UPDATE_DELTA = 0.1;
const float MAX_TEMPERATURE_VALUE = 40;
const float MIN_TEMPERATURE_VALUE = -40;

const float MINIMUM_HUMIDITY_UPDATE_DELTA = 1;
const float MAX_HUMIDITY_VALUE = 100;
const float MIN_HUMIDITY_VALUE = 0;

const int INTERVAL_1_HOUR_MILLIS = 3600000;
const int INTERVAL_1_MIN_MILLIS = 60000;
const int INTERVAL_1_SEC_MILLIS = 1000;

#endif
