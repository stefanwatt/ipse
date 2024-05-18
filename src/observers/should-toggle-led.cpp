#include "src/config.h"
#include "should-toggle-led.h"

bool infraredOn = false;
unsigned long enteredAt = 0;

bool ShouldToggleLed(const unsigned long now, bool updatedInfraredOn){
  if(!updatedInfraredOn){
    enteredAt = 0;
    return false;
  }

  if (!infraredOn){
    enteredAt = now;
    return false;
  }

  const int timeDiff = now - enteredAt;

  if (timeDiff < LED_TOGGLE_DURATION) return false;

  enteredAt = 0;
  return true;
}
