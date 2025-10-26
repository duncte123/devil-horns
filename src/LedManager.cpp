#include "LedManager.h"
#include <Arduino.h>

LEDManager::LEDManager() {}

void LEDManager::begin() {
    // Get the size of the array, cuz we're not on cpp 17
    int n = sizeof(leds) / sizeof(leds[0]);

    for (int i = 0; i < n; i++) {
        int ledPin = leds[i];

        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, LOW);
        ledStates[ledPin] = false;
    }

    /*for (auto state : ledStates) {
        pinMode(state.first, OUTPUT);
        digitalWrite(state.first, LOW);
        ledStates[state.first] = false;
    }*/
}

void LEDManager::set(int pin, bool state, bool callback) {
    if (ledStates[pin] != state) {
        ledStates[pin] = state;
        digitalWrite(pin, state ? HIGH : LOW);

        if (callback && onLEDStateChanged) {
            onLEDStateChanged(pin, state);
        }
    }
}

void LEDManager::flash(int pin, int times, int onTime, int offTime) {
    for (int t = 0; t < times; t++) {
        digitalWrite(pin, HIGH);
        delay(onTime);
        digitalWrite(pin, LOW);
        delay(offTime);
    }
}

bool LEDManager::get(int pin) { return ledStates[pin]; }

void LEDManager::setOnLEDStateChanged(std::function<void(int, bool)> callback) {
    onLEDStateChanged = callback;
}
