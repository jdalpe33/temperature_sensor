#ifndef DHT22_DEF
#define DHT22_DEF

#include "DHT.h"

#define DHTPIN 16
#define DHTTYPE DHT22 

class TemperatureDHT22 {
  private:
    DHT* dht;
  public:
    void initialize();
    
    float getTemperature();
    float getHumidity();
    float getHeatIndex();
};

void TemperatureDHT22::initialize() {
  dht = new DHT(DHTPIN, DHTTYPE);
  dht->begin();
}

float TemperatureDHT22::getTemperature() {
  return dht->readTemperature();
}

float TemperatureDHT22::getHumidity() {
  return dht->readHumidity();
}

float TemperatureDHT22::getHeatIndex() {
  return dht->computeHeatIndex(getTemperature(), getHumidity(), false);
}

#endif
