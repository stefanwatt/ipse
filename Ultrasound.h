#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include "Arduino.h"

class Ultrasound {
public:
    Ultrasound(int trigPin, int echoPin);
    long getDistance();

private:
    int _trigPin;
    int _echoPin;
};

#endif
