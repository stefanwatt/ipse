#include "src/config.h"
#include "src/infrared/infrared.h"
#include "src/led/led.h"
#include "src/observers/should-toggle-led.h"
#include "src/pinout.h"
#include "src/ultrasound/ultrasound.h"

Ultrasound ultrasound(ULTRASOUND_TRIG_PIN, ULTRASOUND_ECHO_PIN, MAX_DISTANCE,
                      MIN_DISTANCE);

Led led(LED_PIN, FADE_DURATION, MAX_BRIGHTNESS, DEFAULT_LED_PERCENTAGE);

Infrared infrared(INFRARED_PIN);

void setup() { Serial.begin(9600); }

void loop() {
  const unsigned long now = millis();
  if (ShouldToggleLed(now, infrared.IsTriggered()))
    led.Toggle();
  if (false) {
    const int percentage = ultrasound.GetDistancePercentage();
    led.UpdateBrightness(percentage);
  }
}
