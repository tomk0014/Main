
// // DS18B20.h

#ifndef DS18B20Sensor_h
#define DS18B20Sensor_h

#include <OneWire.h>
#include <DallasTemperature.h>

class DS18B20Sensor {
  public:
    DS18B20Sensor(int pin); // Constructor with pin number
    void begin();           // Initializes the sensor
    float readTemperature(); // Reads the temperature in Celsius

  private:
    OneWire _oneWire;            // OneWire instance to communicate with sensor
    DallasTemperature _sensors;  // DallasTemperature instance for sensor
    int _pin;                    // Pin number where the data wire is connected
};

#endif
