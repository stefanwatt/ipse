#ifndef LED_OBSERVER_H
#define LED_OBSERVER_H

#include "Arduino.h"

class LedObserver {
  public:
    LedObserver();
    bool ShouldToggleLed(const unsigned long now, const bool updatedInfraredActive);
  private:
    unsigned long enteredAt=0;
    unsigned long toggledAt=0;
    bool infraredOn=false;
};
#endif

