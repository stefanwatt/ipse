#include "Led.h"
#include "Ultrasound.h"

int trigPin = 2;
int echoPin = 3;
int ledPin = 3;

int radarPin = 4;
int radarVal = 0;

Ultrasound ultrasound(trigPin, echoPin);
Led led(ledPin);
unsigned long lastExecution = 0;
void setup() { Serial.begin(9600); }

void loop() {
  unsigned long time_now = millis();
  const int percentage = ultrasound.getDistancePercentage();
  led.UpdateBrightness(percentage);
}
