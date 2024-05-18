
void Ultrasound::tick(unsigned long now){
  _distance = computeDistance();
  if (distance > 15.0) {
    distance = 15.0;
  }
  if (enteredDimmingRange != 0 && !isInDimmingRange(distance)) {
    Serial.print("in dimming range");
  } else {
    Serial.print("distance = ");
    Serial.print(distance);
    Serial.print(" cm");
  }
  Serial.println("");

  double movementDifference = abs(enteredDimmingRangeAtDistance - distance);

  bool shouldToggleDimmingMode =
      ledOn && enteredDimmingRange != 0 &&
      (now - enteredDimmingRange) >= 2000 &&
      movementDifference < movementTolerance &&
      (lastToggledDimmingMode == 0 || now - lastToggledDimmingMode > 10000);
  if (shouldToggleDimmingMode) {
    int updatedBrightness = mapDistanceToBrightness(distance);
    Serial.println("updating brightness to : ");
    Serial.print(updatedBrightness);
    blink();
    isDimming = !isDimming;
    lastToggledDimmingMode = now;
    enteredDimmingRange = 0;
    analogWrite(ledPin, updatedBrightness);
    brightness = updatedBrightness;
    Serial.println("toggled dimming mode");
  }
  bool enteredDimmingRangeRecently =
      ledOn && isInDimmingRange(distance) && enteredDimmingRange == 0;
  if (enteredDimmingRangeRecently) {
    enteredDimmingRangeAtDistance = distance;
    enteredDimmingRange = now;
  } else if (movementDifference > movementTolerance &&
             enteredDimmingRange != 0) {
    enteredDimmingRange = now;
    enteredDimmingRangeAtDistance = distance;
  }

  if (ledOn && isDimming) {
    const int updatedBrightness = mapDistanceToBrightness(distance);
    UpdateLedSmoothly(brightness, updatedBrightness, 100);
    Serial.println("dimming to brightness: ");
    Serial.print(brightness);
  }

}

