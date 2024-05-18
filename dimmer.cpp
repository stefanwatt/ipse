#include "dimmer.h"
#include "config.h"

Dimmer::Dimmer(const Led& _led) : led(_led) {}

void Dimmer::Toggle(const unsigned long now){
    led.Blink();
    lastToggledAt = now;
    Dimming = !Dimming;
}

void Dimmer::Dim(const double _updatedPercentage){
    const double updatedPercentage = DIMMER_ALPHA * _updatedPercentage + (1 - DIMMER_ALPHA) * percentage;
    led.UpdateBrightness(updatedPercentage);
}

