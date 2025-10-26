#include "LedManager.h"
#include <Arduino.h>

LEDManager::LEDManager() = default;

void LEDManager::begin() {
    // Get the size of the array, cuz we're not on cpp 17
    int n = sizeof(leds) / sizeof(leds[0]);

    for (int i = 0; i < n; i++) {
        int ledPin = leds[i];

        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, LOW);
        ledStates[ledPin] = false;
    }
}

void LEDManager::set(int pin, bool state, bool callback) {
    if (!state) {
        blinkStates.erase(pin);
    }

    if (ledStates[pin] != state) {
        setInternally(pin, state);

        if (callback && onLEDStateChanged) {
            onLEDStateChanged(pin, state);
        }
    }
}

void LEDManager::setInternally(int pin, bool state) {
    ledStates[pin] = state;
    digitalWrite(pin, state ? HIGH : LOW);
}

void LEDManager::flash(int pin, int times, int onTime, int offTime) {
    BlinkState newState{
        .previousMillis = millis(),
        .onTime =  onTime,
        .offTime = offTime,
        .blinkCount = 0,
        .targetCount = times,
    };

    blinkStates[pin] = newState;

    /*for (int t = 0; t < times; t++) {
        digitalWrite(pin, HIGH);
        delay(onTime);
        digitalWrite(pin, LOW);
        delay(offTime);
    }*/
}

void LEDManager::handleFlash() {
    for (auto &entry : blinkStates) {
        unsigned long currentMillis = millis();
        int pin = entry.first;
        BlinkState &state = entry.second;

        // Stop if finished desired number of blinks
        if (state.blinkCount >= state.targetCount) {
            blinkStates.erase(pin);
            continue;
        }

        // Turn ON if currently OFF and it's time
        if (get(pin) == LOW && (currentMillis - state.previousMillis >= state.offTime)) {
            setInternally(pin, HIGH);
            state.previousMillis = currentMillis;
        }
            // Turn OFF if currently ON and it's time
        else if (get(pin) == HIGH && (currentMillis - state.previousMillis >= state.onTime)) {
            setInternally(pin, LOW);
            state.previousMillis = currentMillis;
            state.blinkCount++;  // Completed one ON→OFF cycle
        }
    }
}

bool LEDManager::get(int pin) { return ledStates[pin]; }

void LEDManager::setOnLEDStateChanged(std::function<void(int, bool)> callback) {
    onLEDStateChanged = callback;
}
