#ifndef DIMMER_H
#define DIMMER_H

#include "src/led/led.h"
#include "Arduino.h"

class Dimmer {
public:
    Dimmer();
    void ToggleDimming(const unsigned long now);
    void Dim(double updatedPercentage);

private:
    Led led;
    unsigned long lastToggledAt = 0;
};

#endif
