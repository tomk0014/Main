
// 5 Volt with 3 Analogue Pins
// Carbon Monoxide (CO), Nitrogen Dioxide (N02), Ammonia (NH3)

// CJMCU6814.cpp
#include "CJMCU6814.h"

CJMCU6814::CJMCU6814(int pinNO2, int pinNH3, int pinCO)
: _pinNO2(pinNO2), _pinNH3(pinNH3), _pinCO(pinCO) {}

void CJMCU6814::begin() {
  // Initialization code here (if necessary)
}

float CJMCU6814::readNO2_ppm() {
  int value = analogRead(_pinNO2);
  return (value - baselineNO2) * sensitivityNO2;
}

float CJMCU6814::readNH3_ppm() {
  int value = analogRead(_pinNH3);
  return (value - baselineNH3) * sensitivityNH3;
}

float CJMCU6814::readCO_ppm() {
  int value = analogRead(_pinCO);
  return (value - baselineCO) * sensitivityCO;
}
