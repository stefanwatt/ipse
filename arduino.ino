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

void setup()
{
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

void loop()
{

  // Sensorstatus lesen
  irStatus = digitalRead(irPin);

  // loop Radar
  radarVal = digitalRead(radarPin);

  unsigned long time_now = millis();

  // Überprüfen, ob der Sensorstatus sich geändert hat
  if (irStatus != lastIrStatus)
  {
    // Zeitstempel aktualisieren
    lastHMI = time_now;
    // Status aktualisieren
    lastIrStatus = irStatus;
  }

  if (!ledOn && radarVal && irStatus)
  {
    StandBy();
  }

  if (!ledOn && !irStatus)
  { // Einschalten über IR
    if ((time_now - lastHMI) >= 1000)
    {
      EinAusschalten();
      analogWrite(ledPin, 64);
      ledOn = true;
      delay(500);
    }
  }
  else if (ledOn && !irStatus)
  { // Ausschalten über IR
    if ((time_now - lastHMI) >= 2000)
    {
      EinAusschalten();
      ledOn = false;
      delay(500);
    }
  }

  // loop US
  digitalWrite(trigPin, HIGH);
  // delay(1);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  double movementDifference = abs(enteredDimmingRangeAtDistance - distance);

  if (ledOn && enteredDimmingRange != 0 && timeDifference(time_now, enteredDimmingRange) >= 4000 && isDifferenceSmallerThan(distance, enteredDimmingRangeAtDistance, 2.0))
  {
    ConfirmDimmingToggled();
    isDimming = !isDimming;
  }

  if (ledOn && isInDimmingRange(distance) && enteredDimmingRange == 0)
  {
    enteredDimmingRangeAtDistance = distance;
    enteredDimmingRange = time_now;
  }

  if (ledOn && isDimming)
  {
    double finalBrightness = mapDistanceToBrightness(distance);
    if (!updatingSmoothly)
    {
      UpdateLedSmoothly(brightness, finalBrightness, 100);
    }
  }
}

unsigned long timeDifference(unsigned long startTime, unsigned long endTime)
{
  return endTime - startTime;
}

bool isDifferenceSmallerThan(double value1, double value2, double threshold)
{
  return abs(value1 - value2) < threshold;
}

bool isInDimmingRange(double distance)
{
  return distance >= 5.0 && distance <= 30.0;
}

int mapDistanceToBrightness(double distance)
{
  if (distance <= 5.0)
  {
    return 5;
  }
  else if (distance >= 30.0)
  {
    return 64;
  }
  else
  {
    double brightness = 5 + (distance - 5) * (59.0 / 25.0);
    return static_cast<int>(round(brightness));
  }
}

void ConfirmDimmingToggled()
{
  ledUpdateBlocked = true;
  int blinkDuratiion = 300;
  UpdateLedSmoothly(5, 64, blinkDuratiion, true);
  delay(100);
  UpdateLedSmoothly(64, 5, blinkDuratiion, true);
  UpdateLedSmoothly(5, 64, blinkDuratiion, true);
  delay(100);
  UpdateLedSmoothly(64, 5, blinkDuratiion, true);
  ledUpdateBlocked = false;
}

void UpdateLedSmoothly(int startBrightness, int endBrightness, int duration, bool override = false)
{
  if (!override && ledUpdateBlocked)
  {
    return;
  }
  updatingSmoothly = true;
  const int steps = duration / 20; // Duration divided into steps of 20 milliseconds
  for (int step = 0; step <= steps; step++)
  {
    float t = float(step) / float(steps); // Normalized time from 0 to 1
    // Bezier control points
    float p0 = startBrightness;
    float p1 = startBrightness + (endBrightness - startBrightness) * 0.3; // First control point
    float p2 = startBrightness + (endBrightness - startBrightness) * 0.7; // Second control point
    float p3 = endBrightness;

    // Calculate the Bezier curve point
    float brightness = (1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t * t * p2 + t * t * t * p3;
    analogWrite(ledPin, int(brightness));
    delay(20);
  }
  updatingSmoothly = false;
}

// TODO: update to use UpdateLedSmoothly
void StandBy()
{
  for (int i = 1; i <= 64; i = i + 1)
  {
    analogWrite(ledPin, i);
    delay(20);
  }
  for (int i = 64; i >= 0; i = i - 1)
  {
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

void intEinAus()
{
  intPin = true;
}

void EinAusschalten()
{
  analogWrite(ledPin, 64);
  delay(100);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 64);
  delay(100);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 64);
  delay(100);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 64);
  delay(100);
  analogWrite(ledPin, 0);
}