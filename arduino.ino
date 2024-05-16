#include <cmath>

// Pin-Definitionen
int ledPin = 9; // Pin für die LED
int irPin = 10; // Pin für den Infrarotsensor

// Variablen US
int trigPin = 2;
int echoPin = 3;
long duration;
double distance;
int brightness = 5;
bool isDimming = false;
unsigned long enteredDimmingRange = 0;
double enteredDimmingRangeAtDistance = 0.0;

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

  double movementDifference = std::abs(enteredDimmingRangeAtDistance - distance);

  if (ledOn && enteredDimmingRange != 0 && timeDifference(time_now, enteredDimmingRange) >= 2000 && isDifferenceSmallerThan(distance, enteredDimmingRangeAtDistance, 2.0))
  {
    isDimming = !isDimming;
  }

  if (ledOn && isInDimmingRange(distance) && enteredDimmingRange == 0)
  {
    enteredDimmingRangeAtDistance = distance;
    enteredDimmingRange = time_now;
  }

  if (ledOn && isDimming)
  {
    brightness = mapDistanceToBrightness(distance);
    analogWrite(ledPin, brightness);
  }
}

unsigned long timeDifference(unsigned long startTime, unsigned long endTime)
{
  return endTime - startTime;
}

bool isDifferenceSmallerThan(double value1, double value2, double threshold)
{
  return std::abs(value1 - value2) < threshold;
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
    return static_cast<int>(std::round(brightness));
  }
}

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