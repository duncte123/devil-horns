#include "LedManager.h"
#include "NetworkManager.h"
#include "Webserver.h"
#include "Arduino.h"

LEDManager LED;
NetworkManager Network(LED);
Webserver webserver(LED);

void setup() {
    Serial.begin(115200);
    LED.begin();
    Network.connectWiFi();
    Network.connectMQTT();
//    webserver.begin();
}

void loop() {
//    LED.set(LED_HORN, true, false);
//    delay(500);
//    LED.set(LED_HORN, false, false);
//    delay(500);

    Network.loop();
//    webserver.handleRequest();
    LED.handleFlash();
}