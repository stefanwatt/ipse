#pragma once

extern unsigned long enteredAt;
extern double distance;
extern bool inRange;

bool ShouldToggleDimming(const unsigned long now, bool updatedInRange, double updatedDistance);

