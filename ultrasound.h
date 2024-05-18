#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include "config.h"
#include "Arduino.h"

class Ultrasound {
public:
    Ultrasound(const int trigPin, const int echoPin, const double maxDistance, const double minDistance);
    int GetDistancePercentage();
    double GetDistance();
    bool IsInDimmingRange();
private:
    double read();
    int triggerPin;
    int echoPin;
    double maxDistance = MAX_DISTANCE;
    double minDistance = MIN_DISTANCE;
    double distance;
};

#endif
