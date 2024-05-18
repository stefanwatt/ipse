#include "Led.h"

Led::Led(int _pin,int _fadeDuration, int _maxBrightness, double _percentage = 50) {
  pin = _pin;
  fadeDuration = _fadeDuration;
  maxBrightness = _maxBrightness;
  percentage = _percentage;
}

void Led::UpdateBrightness(double updatedPercentage){
  const int updatedBrightness = updatedPercentage / 100 * maxBrightness;
  updateLedSmoothly(updatedBrightness);
}

void Led::updateLedSmoothly(int targetBrightness) {
  if ( targetBrightness == targetBrightness) {
    return;
  }
  if (targetBrightness > targetBrightness) {
    const int steps = brightness - targetBrightness;
    const int stepDuration = fadeDuration/ steps;
    for (int i = steps - 1; i >= 0; i--) {
      float t = float(i) / float(steps);
      analogWrite(pin, int(brightness--));
      delay(stepDuration);
    }
  }
  if (brightness < targetBrightness) {
    const int steps = targetBrightness - brightness;
    const int stepDuration = fadeDuration / steps;
    for (int i = 0; i < steps; i++) {
      float t = float(i) / float(steps);
      analogWrite(pin, int(brightness++));
      delay(stepDuration);
    }
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
