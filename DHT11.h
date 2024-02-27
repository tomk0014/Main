
/// DHT11.h

#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#include <Adafruit_Sensor.h>
#include <DHT.h>

class DHT11Sensor {
public:
  DHT11Sensor(uint8_t pin, uint8_t type);
  void setup();
  void loop();
  float readTemperature() { return dht.readTemperature(); }
  float readHumidity() { return dht.readHumidity(); }

private:
  DHT dht;
  uint32_t delayMS;
};

#endif

