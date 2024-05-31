#include "config.h"
#include "dimmer.h"
#include "dimming-observer.h"
#include "infrared.h"
#include "led-observer.h"
#include "led.h"
#include "pinout.h"
#include "ultrasound.h"

Ultrasound *ultrasound;
Led *led;
LedObserver *ledObserver;
DimmingObserver *dimmingObserver;
Dimmer *dimmer;
Infrared *infrared;

void setup() {
  Serial.begin(9600);
  ultrasound = new Ultrasound(ULTRASOUND_TRIG_PIN, ULTRASOUND_ECHO_PIN,
                              MAX_DISTANCE, MIN_DISTANCE);
  led = new Led(LED_PIN, FADE_DURATION, MAX_BRIGHTNESS, DEFAULT_LED_PERCENTAGE);

  ledObserver = new LedObserver();

  dimmingObserver = new DimmingObserver();

  dimmer = new Dimmer(*led);

  infrared = new Infrared(INFRARED_PIN);
}

void loop() {
  const unsigned long now = millis();

  bool shouldToggleLed =
      ledObserver->ShouldToggleLed(now, infrared->IsActive());

  if (shouldToggleLed) {
    led->Toggle();
  }

  const double distance = ultrasound->GetDistance();

  const bool isInDimmingRange = ultrasound->IsInDimmingRange();
  if (led->On &&
      dimmingObserver->ShouldToggleDimming(now, distance, isInDimmingRange)) {
    dimmer->Toggle(now);
  }

  Serial.println("distance: " + String(distance));
  if (dimmer->Dimming) {
    const int percentage = ultrasound->GetDistancePercentage();
    dimmer->Dim(percentage);
  }
}
