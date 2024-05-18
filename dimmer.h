#ifndef DIMMER_H
#define DIMMER_H

#include "led.h"
#include "Arduino.h"

class Dimmer {
public:
    Dimmer(const Led& _led);
    void Toggle(const unsigned long now);
    void Dim(const double updatedPercentage);
    bool Dimming = false;

private:
    Led led;
    unsigned long lastToggledAt = 0;
    double percentage = 0;
};

#endif
