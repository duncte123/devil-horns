#include "LedManager.h"
#include "NetworkManager.h"
#include "Arduino.h"

LEDManager LED;
NetworkManager Network(LED);

void setup() {
    Serial.begin(115200);
    LED.begin();
    Network.connectWiFi();
}

void loop() {
    LED.set(LED_HORN, true, false);
    delay(500);
    LED.set(LED_HORN, false, false);
    delay(500);
}