// DS18B20 Water Temperature
// 5 Volt Digital Pin
// DS18B20.cpp

#include "DS18B20.h"

DS18B20Sensor::DS18B20Sensor(int pin) : _oneWire(pin), _sensors(&_oneWire), _pin(pin) {
  // Constructor implementation is here. It initializes the OneWire and DallasTemperature instances.
}

void DS18B20Sensor::begin() {
  _sensors.begin(); // Start up the library
}

float DS18B20Sensor::readTemperature() {
  _sensors.requestTemperatures(); // Send the command to get temperatures
  float temperatureC = _sensors.getTempCByIndex(0); // Read temperature in Celsius
  return temperatureC; // Return the temperature reading
}
