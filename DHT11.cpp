// 3.3 Volt Digital
// Temperature and Humidity
// DHT11.cpp
#include "DHT11.h"
#include <Arduino.h>

DHT11Sensor::DHT11Sensor(uint8_t pin, uint8_t type) : dht(pin, type) {}

void DHT11Sensor::setup() {
  dht.begin();
  delayMS = 2000; // Set the delay between sensor readings as needed.
}

void DHT11Sensor::loop() {
  // Delay between measurements is handled in the main sketch loop to avoid blocking calls
}

