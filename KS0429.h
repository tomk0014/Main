// KS0429-TDS value
#ifndef KS0429_H
#define KS0429_H

#include <Arduino.h>

class KS0429 {
public:
    KS0429(uint8_t pin, float vref = 5.0);
    void begin();
    float readTDS();
    void setTemperature(float temperature); // Method to update temperature

private:
    uint8_t _TdsSensorPin;
    float _VREF;
    float _temperature;
    static const int _SCOUNT = 30;
    int _analogBuffer[_SCOUNT];
    int getMedianNum(int bArray[], int iFilterLen);
};

#endif
