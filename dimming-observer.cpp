#include "config.h"
#include "ultrasound.h"
#include "dimming-observer.h"

DimmingObserver::DimmingObserver(){}

bool DimmingObserver::ShouldToggleDimming(const unsigned long now, const double updatedDistance, const bool updatedInDimmingRange){
  if(!updatedInDimmingRange){
    inDimmingRange = false;
    return false;
  }
  
  if(!inDimmingRange){
    enteredAt = now;
    inDimmingRange = true;
    entryDistance = updatedDistance;
    return false;
  }

  const double movementDifference = abs(entryDistance - updatedDistance);
  if(movementDifference > MOVEMENT_TOLERANCE ){
    enteredAt = 0;
    inDimmingRange = false;
    return false;
  }


  const bool shouldToggleDimmingMode =
      enteredAt != 0 &&
      (now - enteredAt) >= DIM_TOGGLE_DURATION;

  const bool isBeforeTimeout = lastToggledAt != 0 &&( now - lastToggledAt) < DIM_TOGGLE_TIMEOUT;
  if(!shouldToggleDimmingMode || isBeforeTimeout){
    return false;
  }

  enteredAt = 0;
  return true;
}

