#ifndef DEVIL_HORNS_WEBSERVER_H
#define DEVIL_HORNS_WEBSERVER_H

#include "LedManager.h"
#include "Arduino.h"
#include <WiFi.h>

class Webserver {
public:
    Webserver(LEDManager &ledRef);
    void begin();
    void handleRequest();

private:
    WiFiServer server;
    LEDManager &LED;

    unsigned long currentTime = millis();
    // Previous time
    unsigned long previousTime = 0;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 2000;

    void registerRoutes();
    void handleLedChange(int led, bool status);
};

#endif //DEVIL_HORNS_WEBSERVER_H
