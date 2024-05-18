#include "infrared.h"

Infrared::Infrared(int _pin){
  pin = _pin;
}

bool Infrared::IsTriggered(){
  return digitalRead(pin) == 1;
}
