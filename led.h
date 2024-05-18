#ifndef LED_H
#define LED_H

#include "Arduino.h"

class Led {
public:
    Led(const int pin, const int fadeDuration, const int maxBrightness, const double defaultPercentage);
    void UpdateBrightness(const double percentage);
    void Blink();
    void TurnOn();
    void TurnOff();
    void Toggle();
    bool On = false;

private:
    int pin;
    int fadeDuration;
    int maxBrightness;
    double percentage;
    double defaultPercentage;
    int brightness = 0;
};

#endif
