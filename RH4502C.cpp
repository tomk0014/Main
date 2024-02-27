

// RH-4502C.cpp

#include "RH4502C.h"

RH4502C::RH4502C(uint8_t pin) : _pin(pin) {}

void RH4502C::begin() {
    // Initialize anything if needed, for pH sensor mostly it's about setting the pin mode
    pinMode(_pin, INPUT);
}

float RH4502C::readPH() {
    int sensorValue = analogRead(_pin);
    float voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
    // Assuming 4.98V corresponds to pH 7 (neutral) and each 0.1V change equals one pH unit...
    const float OFFSET = 0.02; // Adjusted based on your calibration
    const float SCALE_FACTOR = -1.0 / 0.1; // Example scale factor, needs actual calibration
    float phValue = 7.0 + (voltage - (5.0 - OFFSET)) * SCALE_FACTOR;
    return phValue;
}


