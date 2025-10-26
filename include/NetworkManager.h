#ifndef DEVIL_HORNS_NETWORKMANAGER_H
#define DEVIL_HORNS_NETWORKMANAGER_H

#include "Arduino.h"
 #include "LedManager.h"
// #include <PubSubClient.h>
#include <WiFi.h>

class NetworkManager {
public:
    NetworkManager(LEDManager &ledRef);
    void connectWiFi();
    void connectMQTT();
    void loop();
    void messageReceived(char *topic, byte *payload, unsigned int length);
    void onLEDStateChanged(int index, bool state);

private:
     LEDManager &LED;
    WiFiClient wifiClient;
    // PubSubClient mqttClient{wifiClient};

    const char *ssid = "hackalot";
    const char *password = "zegikniet";
    const char *mqttServer = "mqtt.space.hackalot.nl";
    const int mqttPort = 1883;
};

#endif
