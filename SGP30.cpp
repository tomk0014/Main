// Volative Organic Compounds
// Carbon Dioxide (CO2) and Formaldehyde 
// SGP30.cpp
// Multiplex Sensor 3.3v - Channel 0
// Could take 24 hours to warm-up

// SGP30.cpp

#include <Arduino.h>
#include "SGP30.h"

extern void tcaSelect(uint8_t i);
//tcaSelect(0x58); // channel_for_sensor should be replaced with the actual channel number


// SGP30 I2C commands
#define SGP30_MEASURE_AIR_QUALITY 0x2008
#define SGP30_SET_HUMIDITY 0x2061
#define SGP30_I2C_ADDRESS 0x58 // Default I2C address of SGP30

SGP30::SGP30() : TVOC_ppb(0), eCO2_ppm(0) {}

bool SGP30::begin() {
    Wire.begin();
    // You can add more initialization code here
    return true; // For simplicity, assuming init is always successful
}

void SGP30::writeCommand(uint16_t command) {
    Wire.beginTransmission(SGP30_I2C_ADDRESS);
    Wire.write(command >> 8);
    Wire.write(command & 0xFF);
    Wire.endTransmission();
}

bool SGP30::readData(uint8_t *data, uint8_t len) {
    Wire.requestFrom(SGP30_I2C_ADDRESS, len);
    for (uint8_t i = 0; i < len; i++) {
        if (Wire.available()) {
            data[i] = Wire.read();
        } else {
            return false; // Failed to receive data
        }
    }
    return true;
}

uint8_t SGP30::generateCRC(uint8_t *data, uint8_t len) {
    uint8_t crc = 0xFF; // Initial value
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i]; // XOR byte into least sig. byte of crc
        for (uint8_t j = 8; j > 0; --j) { // Loop over each bit
            if (crc & 0x80) crc = (crc << 1) ^ 0x31u;
            else crc = (crc << 1);
        }
    }
    return crc; // Final byte value is the CRC
}

bool SGP30::IAQmeasure() {
    writeCommand(SGP30_MEASURE_AIR_QUALITY);
    delay(50); // Wait for measurement to be taken
    uint8_t data[6];
    if (readData(data, 6)) {
        // Validate CRC for both pairs of data
        if (generateCRC(data, 2) == data[2] && generateCRC(data + 3, 2) == data[5]) {
            eCO2_ppm = (uint16_t)data[0] << 8 | data[1];
            TVOC_ppb = (uint16_t)data[3] << 8 | data[4];
            return true;
        }
    }
    return false;
}

uint16_t SGP30::getTVOC() {
    return TVOC_ppb;
}

uint16_t SGP30::geteCO2() {
    return eCO2_ppm;
}

void SGP30::setHumidity(uint32_t absoluteHumidity) {
    if (absoluteHumidity == 0) {
        // Disabling humidity compensation if the value is 0
        writeCommand(0x2060); // Specific command to disable humidity compensation
        return;
    }
    uint8_t data[3];
    data[0] = (absoluteHumidity >> 8) & 0xFF;
    data[1] = absoluteHumidity & 0xFF;
    data[2] = generateCRC(data, 2);
    Wire.beginTransmission(SGP30_I2C_ADDRESS);
    Wire.write(SGP30_SET_HUMIDITY >> 8);
    Wire.write(SGP30_SET_HUMIDITY & 0xFF);
    Wire.write(data, 3); // Sending the humidity value with CRC
    Wire.endTransmission();
}
