
// 3.3Volt Analogue
#include "KS0429.h"

KS0429::KS0429(uint8_t pin, float vref) : _TdsSensorPin(pin), _VREF(vref), _temperature(25.0) {}

void KS0429::begin() {
    pinMode(_TdsSensorPin, INPUT);
}

float KS0429::readTDS() {
    int analogBufferIndex = 0;
    for (int i = 0; i < _SCOUNT; i++) {
        _analogBuffer[i] = analogRead(_TdsSensorPin);
        delay(10); // Small delay between readings
    }
    int avgValue = getMedianNum(_analogBuffer, _SCOUNT);
    float voltage = avgValue * (_VREF / 1024.0);
    float compensationCoefficient = 1.0 + 0.02 * (_temperature - 25.0);
    float compensationVoltage = voltage / compensationCoefficient;
    float tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
    return tdsValue;
}

void KS0429::setTemperature(float temperature) {
    _temperature = temperature;
}

int KS0429::getMedianNum(int bArray[], int iFilterLen) {
    for (int i = 0; i < iFilterLen - 1; i++) {
        for (int j = 0; j < iFilterLen - i - 1; j++) {
            if (bArray[j] > bArray[j + 1]) {
                int temp = bArray[j];
                bArray[j] = bArray[j + 1];
                bArray[j + 1] = temp;
            }
        }
    }
    if ((iFilterLen & 1) > 0) {
        return bArray[(iFilterLen - 1) / 2];
    } else {
        return (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 - 1]) / 2;
    }
}
