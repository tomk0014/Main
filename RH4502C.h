
// RH-4502C.h

#ifndef RH4502C_H
#define RH4502C_H

#include <Arduino.h>

class RH4502C {
public:
    RH4502C(uint8_t pin);
    void begin();
    float readPH();

private:
    uint8_t _pin;
};

#endif // RH4502C_H






