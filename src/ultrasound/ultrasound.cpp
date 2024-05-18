#include "ultrasound.h"
long duration;
double distance;
bool isDimming = false;
unsigned long enteredDimmingRange = 0;
double enteredDimmingRangeAtDistance = 0.0;
unsigned long lastToggledDimmingMode = 0;
double movementTolerance = 1.5;
const int windowSize = 3;
double ultrasoundReadings[windowSize];
int ultrasoundReadingsIndex = 0;
int numUltrasoundReadings = 0;

Ultrasound::Ultrasound(int _trigPin, int _echoPin, double _maxDistance, double _minDistance) {
    trigPin = _trigPin;
    echoPin = _echoPin;
    maxDistance = _maxDistance;
    minDistance = _minDistance;
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

int Ultrasound::GetDistancePercentage(){
  const double distance = computeDistance();
  return (distance - minDistance) / (maxDistance - minDistance) * 100;
}

long Ultrasound::computeDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;
    const double alpha = 0.4;
    distance = alpha * distance + (1 - alpha) * distance;
    return distance;
}

bool Ultrasound::isInDimmingRange() {
  return distance >= minDistance && distance <= maxDistance;
}
