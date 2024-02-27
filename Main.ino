#include "Wire.h"

#include "BMP280.h"
#include "DHT11.h"
#include "HCSR04.h"
#include "KY018.h"
#include "KS0429.h"
#include "DS18B20.h"
#include "CJMCU6814.h"
#include "SGP30.h"
#include "RH4502C.h"
#define DHT_PIN 2
#define DHT_TYPE DHT11

#define SENSOR_PIN_COUNT 5
int sensorPins[SENSOR_PIN_COUNT] = {A8, A9, A10, A11, A12};
#define TDS_SENSOR_PIN A7

#define NO2_PIN A0
#define NH3_PIN A1
#define CO_PIN A2
CJMCU6814 cjmcu6814(NO2_PIN, NH3_PIN, CO_PIN);

#define NUM_KY018_SENSORS 5
int ky018Pins[NUM_KY018_SENSORS] = {A8, A9, A10, A11, A12};

#define ONE_WIRE_BUS 8


// i2C Multi-plex
#define TCAADDR 0x70

// SGP30
#define SGP30_CHANNEL 0 // Example channel for BMP280
// BMP280
#define BMP280_CHANNEL 1 // Example channel for BMP280
BMP280Sensor bmpSensor;
// BMP280
void tcaSelect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


// RH-4502 - Ph sensor
#define PH_SENSOR_PIN A7
RH4502C phSensor(PH_SENSOR_PIN);


// TIME 
unsigned long lastSensorReadTime = 0;
const unsigned long sensorReadInterval = 60000; // 1min
//const unsigned long sensorReadInterval = 600000; // 10min
//const unsigned long sensorReadInterval = 3600000; // 60min

// Sensor objects

DHT11Sensor dhtSensor(DHT_PIN, DHT_TYPE);
SGP30 sgp;
KS0429 tdsMeter(TDS_SENSOR_PIN);
KY018Sensor ky018Sensors[NUM_KY018_SENSORS] = {
    KY018Sensor(ky018Pins[0]),
    KY018Sensor(ky018Pins[1]),
    KY018Sensor(ky018Pins[2]),
    KY018Sensor(ky018Pins[3]),
    KY018Sensor(ky018Pins[4])
};
DS18B20Sensor mySensor(ONE_WIRE_BUS);




void setup() {
    Serial.begin(9600);
    Wire.begin(); // start i2C communication
    
    while (!Serial);
    dhtSensor.setup();
    HCSR04_initialize();
    tdsMeter.begin();
    mySensor.begin();
    cjmcu6814.begin();
    
    //Multiplex Sensors 3.3v
    // Select the SGP30 channel 0 on TCA9548A
    tcaSelect(SGP30_CHANNEL);
    sgp.begin();
    // Select the BMP280 channel 1 on TCA9548A
    tcaSelect(BMP280_CHANNEL);
    bmpSensor.setup();

    phSensor.begin();

}

// Declaration for calculateAbsoluteHumidity
float calculateAbsoluteHumidity(float temperature, float humidity) {
    const float mw = 18.01534;    // Molar mass of water g/mol
    const float r = 8.31447215;   // Universal gas constant J/mol/K
    const float tk = temperature + 273.15;  // Absolute temperature in K
    float absoluteHumidity = 1000 * mw / r * humidity / 100.0 * exp(17.62 * temperature / (243.12 + temperature)) / tk;
    return absoluteHumidity; // Return absolute humidity in g/m^3
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastSensorReadTime >= sensorReadInterval) {
    lastSensorReadTime = currentTime;
    //bmpSensor.loop(); // Assuming loop method updates internal state

    dhtSensor.loop(); // Similarly for other sensors
    HCSR04_measureDistance();

    // Read temperature and humidity from DHT11
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();

    // Select the channel for the SGP30 sensor
    //tcaSelect(SGP30_CHANNEL);   EXIST just above outside of the looo

    // Ensure valid readings
    if (!isnan(temperature) && !isnan(humidity)) {
      // Calculate absolute humidity
      float absoluteHumidity = calculateAbsoluteHumidity(temperature, humidity);

      // Assuming you have a method to set humidity for the SGP30
      // Convert absolute humidity to the format expected by your SGP30 library if necessary
      sgp.setHumidity(absoluteHumidity);
    }

    sendSensorDataToRaspberryPi(); // Initialize Loop function and print output to serial
  }
}






void sendSensorDataToRaspberryPi() {
  
  // DHT11 data
  Serial.print(F("DHT11-Temperature = "));
  Serial.println(dhtSensor.readTemperature()); //Serial.println(" °C");
  Serial.print(F("DHT11-Humidity = "));
  Serial.println(dhtSensor.readHumidity());//Serial.println(F(" %"));
  
  
  // HCSR04
    float distance = HCSR04_getDistance();
  if (distance != -1) { // Check if distance is valid
    Serial.print("HCSR04-Distance = ");
    Serial.println(distance); //Serial.println(" cm");
  } else {
    Serial.println("HCSR04-Distance: Out of range");
  }

  
  // KY018
  // First, find the minimum resistance to use as a reference
  float minResistance = 1e+37; // Use a very large number to initialize minResistance.

  for (int i = 0; i < NUM_KY018_SENSORS; i++) {
    float resistance = ky018Sensors[i].readResistance();
    if (resistance < minResistance) {
      minResistance = resistance;
    }
  }

  // Now, calculate and print the light intensity percentage for each sensor
  for (int i = 0; i < NUM_KY018_SENSORS; i++) {
    float voltage = ky018Sensors[i].readVoltage();
    float resistance = ky018Sensors[i].readResistance();
    float lightIntensityPercentage = (minResistance / resistance) * 100;

    Serial.print("KY018-");
    Serial.print(i + 1);
    Serial.print("-Voltage = ");
    Serial.println(voltage);
  
    Serial.print("KY018-");
    Serial.print(i + 1);
    Serial.print("-Resistance = ");
    Serial.println(resistance);

    // Print light intensity percentage
    Serial.print("KY018-");
    Serial.print(i + 1);
    Serial.print("-Light Intensity = ");
    Serial.println(lightIntensityPercentage);
  }

  // DS18B20-Water Temperature
  float temperatureC = mySensor.readTemperature(); // Read temperature
  if(temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.print("DS18B20-Temperature: ");
    Serial.println(temperatureC);
  } else {
    Serial.println("Error: Could not read temperature data");
  }


  // Print KS0429-TDS value
  float tdsValue = tdsMeter.readTDS(); // Read TDS value
  Serial.print(F("KS0429-TotalDissolvedSolids = "));
  Serial.println(tdsValue, 0); // Print TDS value in ppm

  // CJMCU-6814 Data
  Serial.print("CJMCU6814-Nitrogen-Dioxide(NO2) = ");
  Serial.println(cjmcu6814.readNO2_ppm());
  Serial.print("CJMCU6814-Amonia(NH3) = ");
  Serial.println(cjmcu6814.readNH3_ppm());
  Serial.print("CJMCU6814-Carbon-Monoxide(CO) = ");
  Serial.println(cjmcu6814.readCO_ppm());
  
  
  // SGP30
  // Read temperature and humidity from DHT11 for the SGP30
  float temperature = dhtSensor.readTemperature();
  float humidity = dhtSensor.readHumidity();

  // Ensure valid readings
  if (!isnan(temperature) && !isnan(humidity)) {
      // Calculate absolute humidity (you'll need to implement this)
      float absoluteHumidity = calculateAbsoluteHumidity(temperature, humidity);

      // Assuming your SGP30 library has a method to set humidity
      sgp.setHumidity(absoluteHumidity);
  }

  // Select the channel from the multiplex for SGP30 and initialize
  tcaSelect(0); // SGP30 is connected to channel 0 (SD0, SC0)

  // Ensure to select the SGP30 channel again if necessary
  tcaSelect(SGP30_CHANNEL); // Select the SGP30 sensor's channel on the TCA9548A

  // Attempt to perform SGP30 air quality measurement
  if (!sgp.IAQmeasure()) {
    Serial.println("Failed to perform SGP30 IAQ measurement");
  } else {
    // If the measurement was successful, read the IAQ data
    uint16_t tvoc_ppb = sgp.getTVOC();
    uint16_t eco2_ppm = sgp.geteCO2();
    Serial.print("SGP30-TVOC = ");
    Serial.println(tvoc_ppb); //ppb
    Serial.print("SGP30-eCO2 = ");
    Serial.println(eco2_ppm); //ppm
  }

  // BMP280  
  // Select the channel for BMP280 and initialize
  tcaSelect(1); // BMP280 is connected to channel 1 (SD1, SC1)

  // BMP280 data
  Serial.print(F("BMP280-Temperature = "));
  Serial.println(bmpSensor.readTemperature()); //Serial.println(" °C");
  Serial.print(F("BMP280-Pressure = "));
  // Pa to hPa
  Serial.println(bmpSensor.readPressure() / 100); //Serial.println(" hPa");
  Serial.print(F("BMP280-Altitude = "));
  Serial.println(bmpSensor.readAltitude()); //Serial.println(" m");

  // RH-4502C
  // Print the pH value
  float phValue = phSensor.readPH();
  Serial.print("RH4502C-pH = ");
  Serial.println(phValue);


}


