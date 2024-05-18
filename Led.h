#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include "Arduino.h"

class Led {
public:
    Led(int pin,  int fadeDuration, int maxBrightness, double percentage);
    void UpdateBrightness(double percentage);
    void Blink();
    void On();
    void Off();

private:
    int pin;
    int fadeDuration;
    int maxBrightness;
    double percentage;
    int brightness;
    void updateLedSmoothly( int endBrightness);
};

#endif
