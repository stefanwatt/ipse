#include "config.h"
#include "led-observer.h"


LedObserver::LedObserver(){}

bool LedObserver::ShouldToggleLed(const unsigned long now, const bool updatedInfraredActive){
  if(!updatedInfraredActive){
    enteredAt = 0;
    return false;
  }

  if (!infraredOn && enteredAt == 0){
    enteredAt = now;
    return false;
  }

  int timeDiff = now - enteredAt;

  if (timeDiff < LED_TOGGLE_DURATION) {
    return false;
  }

  timeDiff = now - toggledAt;
  if(toggledAt != 0 && timeDiff < LED_TOGGLE_TIMEOUT){
    return false;
  }
  toggledAt = now;
  enteredAt = 0;
  return true;
}
