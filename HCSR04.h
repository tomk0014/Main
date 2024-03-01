// HCSR04.h

#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

class HCSR04 {
  public:
    HCSR04(int trigPin, int echoPin); // Constructor with pin assignments
    void initialize();
    void measureDistance();
    float getDistance();

  private:
    int _trigPin;
    int _echoPin;
    float _distance;
};

#endif  // HC_SR04_H
