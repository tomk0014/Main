// 5 Volt Digital
// Distance
// HCSR04.cpp

#include "HCSR04.h"

int trig = 6;
int echo = 7;
float distance;

void HCSR04_initialize() {
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  pinMode(echo, INPUT);
  // Removed initial Serial.begin(9600) and delay(2000) to streamline operation
}

void HCSR04_measureDistance() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  distance = (duration > 0) ? duration / 58.0 : -1; // -1 indicates out of range
  // Removed Serial output, delay(20000) to streamline and non-block execution
}

// Consider adding a function to get the last measured distance
float HCSR04_getDistance() {
  return distance; // Returns the distance in cm
}
