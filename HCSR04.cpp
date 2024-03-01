// 5 Volt Digital
// Distance
// HCSR04.cpp

#include "HCSR04.h"

HCSR04::HCSR04(int trigPin, int echoPin) {
  _trigPin = trigPin;
  _echoPin = echoPin;
}

void HCSR04::initialize() {
  pinMode(_trigPin, OUTPUT);
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  pinMode(_echoPin, INPUT);
}

void HCSR04::measureDistance() {
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  long duration = pulseIn(_echoPin, HIGH);
  _distance = (duration > 0) ? duration / 58.0 : -1; // -1 indicates out of range
}

float HCSR04::getDistance() {
  return _distance; // Returns the distance in cm
}
