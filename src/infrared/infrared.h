#ifndef INFRARED_H
#define INFRARED_H

#include "Arduino.h"

class Infrared {
public:
    Infrared(int pin);
    bool IsTriggered();

private:
    int pin;
};

#endif
