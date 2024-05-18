#ifndef INFRARED_H
#define INFRARED_H

#include "Arduino.h"

class Infrared {
public:
    Infrared(const int pin);
    bool IsActive();

private:
    int pin;
};

#endif
