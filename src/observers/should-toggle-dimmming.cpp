#include "src/config.h"
#include "src/ultrasound/ultrasound.h"
#include "should-toggle-dimming.h"

bool inDimmingRange = false;
unsigned long enteredAt = 0;
unsigned long lastToggledAt = 0;
double entryDistance = -1.0;
double distance = -1.0;

bool ShouldToggleDimming(const unsigned long now, bool updatedInDimmingRange,double distance){
  double movementDifference = abs(entryDistance - distance);

  if(!updatedInDimmingRange){
    enteredAt = 0;
    return false;
  }

  if (!inDimmingRange){
    enteredAt = now;
    return false;
  }

  bool shouldToggleDimmingMode =
      enteredAt != 0 &&
      (now - enteredAt) >= DIM_TOGGLE_DURATION &&
      movementDifference < MOVEMENT_TOLERANCE &&
      (lastToggledAt == 0 || now - lastToggledAt > DIM_TOGGLE_TIMEOUT);

  if(!shouldToggleDimmingMode) return false;

  enteredAt = 0;
  return true;
}

