// 5 Volt Analogue
// Light resistance
// Formula: V=I×R
// KY018.cpp

#include "KY018.h"
#include <Arduino.h>

KY018Sensor::KY018Sensor(int pin) : sensorPin(pin) {
  // Constructor initialization list to properly initialize sensorPin
}

void KY018Sensor::setup() {
}

void KY018Sensor::loop() {
}

float KY018Sensor::readVoltage() {
  int rawValue = analogRead(sensorPin);
  float voltage = rawValue * (5.0 / 1023.0) * 1000; // Convert to millivolts
  return voltage;
}

float KY018Sensor::readResistance() {
  float voltage = readVoltage();
  float resistance = 10000 * (voltage / (5000.0 - voltage)); // Calculate resistance
  return resistance;
}
