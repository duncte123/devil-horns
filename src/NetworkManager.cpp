#include "NetworkManager.h"
#include "LedManager.h"

NetworkManager::NetworkManager(LEDManager &ledRef) : LED(ledRef) {}

void NetworkManager::connectWiFi() {
    Serial.printf("Connecting to Wi-Fi: %s\n", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long startAttempt = millis();

    // Flash LED 0 while attempting to connect to Wi-Fi
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 20000) {
         LED.flash(LED_INTERNAL, 1, 200, 200);
        delay(200);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("Connected to Wi-Fi! IP: %s\n",
                      WiFi.localIP().toString().c_str());
        LED.flash(LED_INTERNAL, 2, 300, 150);
    } else {
        Serial.println("Wi-Fi connection failed!");
        // Keep flashing LED 0 on failure until reset or success
        while (WiFi.status() != WL_CONNECTED) {
            LED.flash(LED_INTERNAL, 1, 200, 200);
            delay(500);
        }
    }
}

void NetworkManager::connectMQTT() {
    while (!mqttClient.connected()) {
        Serial.printf("Connecting to MQTT broker at %s...\n", mqttServer);
        mqttClient.setServer(mqttServer, mqttPort);
        mqttClient.setCallback(
                [this](char *topic, byte *payload, unsigned int length) {
                    this->messageReceived(topic, payload, length);
                });

        if (mqttClient.connect("devil-horns")) {
            Serial.println("MQTT connected!");
            mqttClient.subscribe("hackalot/demo"); // Subscribe to each LED topic
            LED.flash(LED_INTERNAL, 2, 100, 100); // Success flash for LED 1
        } else {
            Serial.printf("MQTT connection failed, rc=%d. Retrying in 2s...\n",
                          mqttClient.state());
            LED.flash(LED_INTERNAL, 1, 200, 200); // Flash on failure for LED 1
            delay(2000);
        }
    }
}

void NetworkManager::loop() {
    if (WiFi.status() != WL_CONNECTED) {
        LED.flash(LED_INTERNAL, 1, 200, 200); // Flash LED 0 one time
        Serial.println("Attempting to reconnect to Wi-Fi...");
        connectWiFi();
        return;
    }

    if (!mqttClient.connected()) {
        LED.flash(LED_INTERNAL, 1, 200, 200); // Flash LED 1 one time
        Serial.println("MQTT disconnected...");
        connectMQTT();
        return;
    }

    mqttClient.loop();
}

void NetworkManager::messageReceived(char *topic, byte *payload,
                                     unsigned int length) {
    String topicStr(topic);
    int ledIndex = topicStr.substring(topicStr.lastIndexOf("/") + 1).toInt();

    if (topicStr == "hackalot/demo") {
        LED.flash(LED_HORN, 5, 1000, 1000);
    }

    if (ledIndex >= 0 && ledIndex < 7) {
        LED.set(ledIndex, payload[0] == '1', false); // Update LED state
    }
}

void NetworkManager::onLEDStateChanged(int index, bool state) {
    /*mqttClient.publish((String("hackalot/closing-checklist/") + index).c_str(),
                       state ? "1" : "0");*/
}
