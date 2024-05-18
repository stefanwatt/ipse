#include "lib/config.h"
#include "lib/led/led.h"
#include "lib/pinout.h"
#include "lib/ultrasound/ultrasound.h"

Ultrasound ultrasound(ULTRASOUND_TRIG_PIN, ULTRASOUND_ECHO_PIN, MAX_DISTANCE,
                      MIN_DISTANCE);

Led led(LED_PIN, FADE_DURATION, MAX_BRIGHTNESS, DEFAULT_LED_PERCENTAGE);

void setup() { Serial.begin(9600); }

void loop() {
  const unsigned long now = millis();
  const int percentage = ultrasound.GetDistancePercentage();
  led.UpdateBrightness(percentage);
}
