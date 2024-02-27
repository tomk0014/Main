// Carbon Monoxide (CO), Nitrogen Dioxide (N02), Ammonia (NH3)
// CJMCU6814.h

#ifndef CJMCU6814_h
#define CJMCU6814_h

#include "Arduino.h"

class CJMCU6814 {
  public:
    CJMCU6814(int pinNO2, int pinNH3, int pinCO);
    void begin();
    float readNO2_ppm();
    float readNH3_ppm();
    float readCO_ppm();
  private:
    int _pinNO2, _pinNH3, _pinCO;
    const int baselineNO2 = 1000;
    const int baselineNH3 = 1000;
    const int baselineCO = 1000;
    const float sensitivityNO2 = 0.1;
    const float sensitivityNH3 = 0.1;
    const float sensitivityCO = 0.1;
};

#endif
