// Multiplex Sensor 3.3v - Channel 0
// SGP30.h

#ifndef SGP30_H
#define SGP30_H

#include <Wire.h>

class SGP30 {
public:
    SGP30();
    bool begin();
    bool IAQmeasure();
    uint16_t getTVOC();
    uint16_t geteCO2();
    void setHumidity(uint32_t absoluteHumidity);

private:
    uint16_t TVOC_ppb, eCO2_ppm;
    void writeCommand(uint16_t command);
    bool readData(uint8_t *data, uint8_t len);
    uint8_t generateCRC(uint8_t *data, uint8_t len);
};

#endif // SGP30_H
