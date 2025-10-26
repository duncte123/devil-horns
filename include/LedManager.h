#ifndef DEVIL_HORNS_LEDMANAGER_H
#define DEVIL_HORNS_LEDMANAGER_H

#include "Arduino.h"
#include <functional>
#include <unordered_map>

#define LED_INTERNAL 2
#define LED_HORN 5

class LEDManager {
public:
    LEDManager();
    void begin();
    void set(int pin, bool state, bool callback);
    void flash(int pin, int times, int onTime, int offTime);
    bool get(int pin);

    void setOnLEDStateChanged(std::function<void(int, bool)> callback);

private:
    int leds[2] = {LED_INTERNAL, LED_HORN};
    std::unordered_map<int, bool> ledStates /*= {(LED_INTERNAL, false), (LED_HORN, false)}*/;
    std::function<void(int, bool)> onLEDStateChanged = nullptr;
};


#endif //DEVIL_HORNS_LEDMANAGER_H
