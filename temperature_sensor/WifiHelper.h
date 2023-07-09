#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <WiFi.h>

class WifiHelper {
  private:
  public:
    void connect();
    void reconnectSequence();
    bool isConnected();
};

void WifiHelper::connect() {
  WiFi.disconnect(true);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  log_i("Connecting to WiFi ..");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    log_i( ".");
    tries++;

    if(tries >= 20) {
      ESP.restart();
    }
    
    delay(1000);
  }

  log_i("Connected to WiFi.");

  randomSeed(micros());
}

void WifiHelper::reconnectSequence() {
  if(WiFi.status() != WL_CONNECTED) {
    log_i("Reconnecting to WiFi...");
    connect();
  }
} 

bool WifiHelper::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

#endif
