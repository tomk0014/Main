// HCSR04.h

#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

extern int trig;
extern int echo;
extern int duration;
extern float distance;
extern float meter;

void HCSR04_initialize();
void HCSR04_measureDistance();
float HCSR04_getDistance();

#endif  // HCSR04_H
