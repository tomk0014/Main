// 3.3 Volt i2C (SDA/SCL)
// Multiplex Sensor 3.3v - Channel 1

// BMP280.cpp
#include "BMP280.h"

void BMP280Sensor::setup() {
  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println(F("BMP280 sensor not detected, check wiring!"));
    while (1); // In a production environment, consider implementing a more graceful error handling strategy.
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void BMP280Sensor::loop() {
  // No operation here. Data reading happens in accessor methods.
  // This method can be used for periodic updates if needed.
}
