#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include "Arduino.h"

class Ultrasound {
public:
    Ultrasound(int trigPin, int echoPin, double maxDistance, double minDistance);
    int GetDistancePercentage();

private:
    long computeDistance();
    long normalizeDistance();
    bool isInDimmingRange();
    int trigPin;
    int echoPin;
    double maxDistance = 15.0;
    double minDistance = 4.0;
    double distance;
};

#endif
