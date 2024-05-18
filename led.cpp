#include "config.h"
#include "led.h"

Led::Led(const int _pin, const int _fadeDuration, const int _maxBrightness, const double _defaultPercentage = 50) {
  pin = _pin;
  fadeDuration = _fadeDuration;
  maxBrightness = _maxBrightness;
  percentage = _defaultPercentage;
  defaultPercentage = _defaultPercentage;
}

void Led::TurnOn(){
  UpdateBrightness(100);
  On = true;
}

void Led::TurnOff(){
  UpdateBrightness(0);
  analogWrite(pin, 0);
  brightness = 0; 
  On = false;
}

void Led::Toggle(){
  if(brightness > 0){
    TurnOff();
  }else{
    UpdateBrightness(defaultPercentage);
    On = true;
  }
}

void Led::UpdateBrightness(const double updatedPercentage){
  const int targetBrightness = updatedPercentage / 100 * maxBrightness;
  if ( brightness == targetBrightness) {
    return;
  }
  const int steps = abs(targetBrightness - brightness);
  const int stepDuration = fadeDuration / steps;
  const int stepDirection = (targetBrightness > brightness) ? 1 : -1;

  for (int i = 0; i < steps; i++) {
    analogWrite(pin, brightness);
    brightness += stepDirection;
    delay(stepDuration);
  }
}

void Led::Blink(){
  analogWrite(pin, 64);
  delay(100);
  analogWrite(pin, 0);
  delay(100);
  analogWrite(pin, 64);
  delay(100);
  analogWrite(pin, 0);
  delay(100);
  analogWrite(pin, brightness);
}
