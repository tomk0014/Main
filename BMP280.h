// BP-280 Temp, Pressure, Altitude
// Multiplex Sensor 3.3v - Channel 1

// BMP280.h
#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP280_ADDRESS 0x76   // <-- ADDRESS

class BMP280Sensor {
public:
  void setup();
  void loop();
  float readTemperature() { return bmp.readTemperature(); }
  float readPressure() { return bmp.readPressure(); }
  float readAltitude(float seaLevelPressure = 1029.00) { return bmp.readAltitude(seaLevelPressure); }

private:
  Adafruit_BMP280 bmp; // I2C
};

#endif
