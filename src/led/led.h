#ifndef LED_H
#define LED_H

#include "Arduino.h"

class Led {
public:
    Led(int pin,  int fadeDuration, int maxBrightness, double defaultPercentage);
    void UpdateBrightness(double percentage);
    void Blink();
    void On();
    void Off();
    void Toggle();

private:
    int pin;
    int fadeDuration;
    int maxBrightness;
    double percentage;
    double defaultPercentage;
    int brightness;
    void updateLedSmoothly( int endBrightness);
};

#endif
