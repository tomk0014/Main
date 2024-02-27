// KY018.h

#ifndef KY018_SENSOR_H
#define KY018_SENSOR_H

class KY018Sensor {
public:
  KY018Sensor(int pin);
  void setup();
  void loop();
  float readVoltage(); // Added method to read voltage
  float readResistance(); // Added method to read resistance

private:
  int sensorPin;
};

#endif
