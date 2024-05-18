#include "dimmer.h"

Dimmer::Dimmer(Led _led){
  led = _led;
}
void Dimmer::ToggleDimming(const unsigned long now){
    led.Blink();
    lastToggledAt = now;
    Serial.println("toggled dimming mode");
}

void Dimmer::Dim(double updatedPercentage){
    Serial.println("updating brightness to : ",updatedPercentage,"%");
    led.UpdateBrightness(updatedPercentage);
}

