#ifndef DIMMING_OBSERVER_H
#define DIMMING_OBSERVER_H

#include "Arduino.h"

class DimmingObserver {
  public:
    DimmingObserver();
    bool ShouldToggleDimming(const unsigned long now, const double updatedDistance, const bool updatedInDimmingRange);
  private:
    bool inDimmingRange = false;
    unsigned long enteredAt;
    unsigned long lastToggledAt = 0;
    bool inRange;
    double entryDistance = -1.0;
};
#endif
