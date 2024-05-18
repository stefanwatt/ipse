#pragma once

extern unsigned long enteredAt;
extern bool infraredOn;

bool ShouldToggleLed(const unsigned long now, bool updatedInfraredOn);
