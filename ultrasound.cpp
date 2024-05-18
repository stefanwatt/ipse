#include "ultrasound.h"
#include "config.h"


Ultrasound::Ultrasound(const int _trigPin, const int _echoPin, const double _maxDistance, const double _minDistance) {
    triggerPin = _trigPin;
    echoPin = _echoPin;
    maxDistance = _maxDistance;
    minDistance = _minDistance;
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

int Ultrasound::GetDistancePercentage() {
    const double distance = GetDistance();  
    double percentage = (distance - minDistance) / (maxDistance - minDistance) * 100;
    return percentage;
}

double Ultrasound::GetDistance() {
    double readings[ULTRASOUND_MEASUREMENTS_PER_CYCLE];
    for (int i = 0; i < ULTRASOUND_MEASUREMENTS_PER_CYCLE; ++i) {
        const long duration = read();
        readings[i] = duration * 0.034 / 2;
        delayMicroseconds(10);
    }

    // sort
    for (int i = 0; i < ULTRASOUND_MEASUREMENTS_PER_CYCLE - 1; i++) {
        for (int j = 0; j < ULTRASOUND_MEASUREMENTS_PER_CYCLE - i - 1; j++) {
            if (readings[j] > readings[j + 1]) {
                double temp = readings[j];
                readings[j] = readings[j + 1];
                readings[j + 1] = temp;
            }
        }
    }

    // sum ignoring outliers (highest and lowest reading)
    double sum = 0;
    for (int i = 1; i < ULTRASOUND_MEASUREMENTS_PER_CYCLE - 1; i++) {
        sum += readings[i];
    }

    const double updatedDistance = sum / (ULTRASOUND_MEASUREMENTS_PER_CYCLE - 2);
    distance = ULTRASOUND_ALPHA * updatedDistance + (1 - ULTRASOUND_ALPHA) * distance;
    return distance;
}

bool Ultrasound::IsInDimmingRange() {
  return distance >= minDistance && distance <= maxDistance;
}

double Ultrasound::read(){
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    return pulseIn(echoPin, HIGH); 
}

