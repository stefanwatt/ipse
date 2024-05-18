#include "infrared.h"

Infrared::Infrared(const int _pin){
  pin = _pin;
}

bool Infrared::IsActive(){
  return digitalRead(pin) == 0;
}
