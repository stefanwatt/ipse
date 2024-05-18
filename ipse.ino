// Pin-Definitionen
int ledPin = 9; // Pin für die LED
int irPin = 10; // Pin für den Infrarotsensor
bool ledUpdateBlocked = false;

// Variablen US
int trigPin = 2;
int echoPin = 3;
long duration;
double distance;
int brightness = 5;
bool isDimming = false;
unsigned long enteredDimmingRange = 0;
double enteredDimmingRangeAtDistance = 0.0;
bool updatingSmoothly = false;
unsigned long lastToggledDimmingMode = 0;
double movementTolerance = 1.5;
const int windowSize = 3;
double ultrasoundReadings[windowSize];
int ultrasoundReadingsIndex = 0;
int numUltrasoundReadings = 0;

// Variablen Radar
int radarPin = 4; // Eingang Radar
int radarVal = 0; // BinärVariable

// Variablen zum Verfolgen des Status und der Zeit
unsigned long lastHMI = 0; // Zeitstempel
bool lastIrStatus = true;  // Letzter bekannter Status des Sensors
bool ledOn = false;        // Status der LED (an/aus)
bool irStatus = false;     // ausgelesener Sensorstatus

// interrupt
bool intPin = false;

void UpdateLedSmoothly(int startBrightness, int endBrightness, int duration,
                       bool override = false) {
  if ((!override && ledUpdateBlocked) || startBrightness == endBrightness ||
      updatingSmoothly) {
    return;
  }
  updatingSmoothly = true;

  if (startBrightness > endBrightness) {
    const int steps = startBrightness - endBrightness;
    const int stepDuration = duration / steps;
    for (int i = steps - 1; i >= 0; i--) {
      float t = float(i) / float(steps);
      analogWrite(ledPin, int(brightness--));
      delay(stepDuration);
    }
  }
  if (startBrightness < endBrightness) {
    const int steps = endBrightness - startBrightness;
    const int stepDuration = duration / steps;
    for (int i = 0; i < steps; i++) {
      float t = float(i) / float(steps);
      analogWrite(ledPin, int(brightness++));
      delay(stepDuration);
    }
  }
  updatingSmoothly = false;
}

double getNormalizedDistance(double newReading) {
  ultrasoundReadings[ultrasoundReadingsIndex] = newReading;
  ultrasoundReadingsIndex = (ultrasoundReadingsIndex + 1) % windowSize;

  if (numUltrasoundReadings < windowSize) {
    numUltrasoundReadings++;
  }

  // Create a copy of the readings array and sort it
  double sortedReadings[windowSize];
  for (int i = 0; i < numUltrasoundReadings; i++) {
    sortedReadings[i] = ultrasoundReadings[i];
  }
  for (int i = 0; i < numUltrasoundReadings - 1; i++) {
    for (int j = i + 1; j < numUltrasoundReadings; j++) {
      if (sortedReadings[i] > sortedReadings[j]) {
        double temp = sortedReadings[i];
        sortedReadings[i] = sortedReadings[j];
        sortedReadings[j] = temp;
      }
    }
  }

  // Return the median value
  if (numUltrasoundReadings % 2 == 0) {
    return (sortedReadings[numUltrasoundReadings / 2 - 1] +
            sortedReadings[numUltrasoundReadings / 2]) /
           2.0;
  } else {
    return sortedReadings[numUltrasoundReadings / 2];
  }
}
void handleUltrasound(unsigned long now) {
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  double updated_distance = duration * 0.034 / 2;
  distance = getNormalizedDistance(updated_distance);
  const double alpha = 0.4;
  distance = alpha * updated_distance + (1 - alpha) * distance;
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

unsigned long timeDifference(unsigned long startTime, unsigned long endTime) {
  return endTime - startTime;
}

bool isInDimmingRange(double distance) {
  return distance >= 5.0 && distance <= 10.0;
}

int mapDistanceToBrightness(double distance) {
  double min = 5.0;
  double max = 12.0;
  int maxBrightness = 100;
  if (distance >= 5.0 && distance <= 6.0) {
    return 10;
  }
  if (distance >= 6.0 && distance <= 7.0) {
    return 30;
  }
  if (distance >= 6.0 && distance <= 7.0) {
    return 40;
  }
  if (distance >= 7.0 && distance <= 8.0) {
    return 50;
  }
  if (distance >= 8.0 && distance <= 9.0) {
    return 60;
  }
  if (distance >= 9.0 && distance <= 10.0) {
    return 70;
  }
  if (distance >= 10.0 && distance <= 11.0) {
    return 80;
  }
  if (distance >= 11.0 && distance <= 12.0) {
    return 90;
  }
  return brightness;
}

void StandBy() {
  for (int i = 1; i <= 64; i = i + 1) {
    analogWrite(ledPin, i);
    delay(20);
  }
  for (int i = 64; i >= 0; i = i - 1) {
    analogWrite(ledPin, i);
    delay(20);
  }
}

// void Einschalten(){
//     for (int i =1; i <= 64; i = i +1){
//     analogWrite(ledPin, i);
//     delay(10);
//   }
//   for (int i =64; i >= 0; i = i -1){
//     analogWrite(ledPin, i);
//     delay(10);
//   }
//     for (int i =1; i <= 128; i = i +1){
//     analogWrite(ledPin, i);
//     delay(10);
//   }
//   for (int i =128; i >= 0; i = i -1){
//     analogWrite(ledPin, i);
//     delay(10);
//   }
//     for (int i =1; i <= 255; i = i +1){
//     analogWrite(ledPin, i);
//     delay(10);
//   }
// }

void intEinAus() { intPin = true; }

void blink() {
  analogWrite(ledPin, 64);
  delay(100);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 64);
  delay(100);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, brightness);
}

void setup() {
  // Pins initialisieren
  pinMode(ledPin, OUTPUT);
  pinMode(irPin, INPUT);

  // pinMode US
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initiale LED ausschalten
  digitalWrite(ledPin, LOW);

  // interrupt
  attachInterrupt(digitalPinToInterrupt(irPin), intEinAus, RISING);

  // Serieller Monitor
  Serial.begin(9600);
}

void loop() {

  // Sensorstatus lesen
  irStatus = digitalRead(irPin);

  // loop Radar
  // radarVal = digitalRead(radarPin);

  unsigned long time_now = millis();

  if (irStatus != lastIrStatus) {
    lastHMI = time_now;
    lastIrStatus = irStatus;
  }

  // if (!ledOn && radarVal && irStatus)
  // {
  //   StandBy();
  // }

  if (!ledOn && !irStatus) { // Einschalten über IR
    if ((time_now - lastHMI) >= 1000) {
      blink();
      analogWrite(ledPin, 64);
      ledOn = true;
      delay(500);
    }
  } else if (ledOn && !irStatus) { // Ausschalten über IR
    if ((time_now - lastHMI) >= 2000) {
      blink();
      ledOn = false;
      delay(500);
    }
  }
  handleUltrasound(time_now);
}
