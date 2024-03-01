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

// DHT11
#define DHT_PIN1 2 // Use DHT_PIN1 for the first DHT11 sensor
#define DHT_PIN2 3 // Define a separate name, DHT_PIN2, for the second DHT11 sensor
#define DHT_TYPE DHT11
DHT11Sensor dhtSensor(DHT_PIN1, DHT_TYPE); // Create the first sensor instance
DHT11Sensor dhtSensor2(DHT_PIN2, DHT_TYPE); // Create the second sensor instance

// HCSR04
HCSR04 hcsr04_1(12, 13);
HCSR04 hcsr04_2(10, 11);
HCSR04 hcsr04_3(8, 9);
HCSR04 hcsr04_4(6, 7);

#define SENSOR_PIN_COUNT 5
int sensorPins[SENSOR_PIN_COUNT] = {A8, A9, A10, A11, A12};
#define TDS_SENSOR_PIN A7

// CJMCU6814
#define NO2_PIN A0
#define NH3_PIN A1
#define CO_PIN A2
CJMCU6814 cjmcu6814(NO2_PIN, NH3_PIN, CO_PIN);

#define NUM_KY018_SENSORS 5
int ky018Pins[NUM_KY018_SENSORS] = {A8, A9, A10, A11, A12};

// DS18B20
#define ONE_WIRE_BUS 5


// i2C Multi-plex
#define TCAADDR 0x70

// SGP30
#define SGP30_CHANNEL 0
SGP30 sgp;

// BMP280
#define BMP280_CHANNEL 1
BMP280Sensor bmpSensor;

// RH-4502 - Ph sensor
#define PH_SENSOR_PIN A7
RH4502C phSensor(PH_SENSOR_PIN);

// TIME 
unsigned long lastSensorReadTime = 0;
const unsigned long sensorReadInterval = 60000; // 1min
//const unsigned long sensorReadInterval = 600000; // 10min
//const unsigned long sensorReadInterval = 3600000; // 60min

// Sensor objects
KS0429 tdsMeter(TDS_SENSOR_PIN);
KY018Sensor ky018Sensors[NUM_KY018_SENSORS] = {
    KY018Sensor(ky018Pins[0]),
    KY018Sensor(ky018Pins[1]),
    KY018Sensor(ky018Pins[2]),
    KY018Sensor(ky018Pins[3]),
    KY018Sensor(ky018Pins[4])
};
DS18B20Sensor mySensor(ONE_WIRE_BUS);

// BMP280
void tcaSelect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


void setup() {
    Serial.begin(9600);
    Wire.begin(); // start i2C communication
    
    while (!Serial);
    dhtSensor.setup();
    dhtSensor2.setup(); // Setup the second DHT11 sensor
    // Initialize all HCSR04 sensors
    hcsr04_1.initialize();
    hcsr04_2.initialize();
    hcsr04_3.initialize();
    hcsr04_4.initialize();
    tdsMeter.begin();
    mySensor.begin();
    cjmcu6814.begin();
    
    //Multiplex Sensors 3.3v
    tcaSelect(SGP30_CHANNEL);
    sgp.begin();
    tcaSelect(BMP280_CHANNEL);
    bmpSensor.setup();

    phSensor.begin();

} // END setup


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

    // Read temperature and humidity from DHT11
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();
    float absoluteHumidity = 0; // Initialize with 0

    // Ensure valid readings
    if (!isnan(temperature) && !isnan(humidity)) {
      absoluteHumidity = calculateAbsoluteHumidity(temperature, humidity);
      tcaSelect(SGP30_CHANNEL); // Ensure SGP30 channel is selected before setting humidity
      sgp.setHumidity(absoluteHumidity);
    }
  
  // HCSR04
  hcsr04_1.measureDistance();
  float distance1 = hcsr04_1.getDistance();
  hcsr04_2.measureDistance();
  float distance2 = hcsr04_2.getDistance();
  hcsr04_3.measureDistance();
  float distance3 = hcsr04_3.getDistance();
  hcsr04_4.measureDistance();
  float distance4 = hcsr04_4.getDistance();

    sendSensorDataToRaspberryPi(absoluteHumidity);
  }
} // END Loop


void sendSensorDataToRaspberryPi(float absoluteHumidity) {
  // DHT11 data
  Serial.print(F("DHT11-Temperature = "));
  Serial.println(dhtSensor.readTemperature()); //Serial.println(" °C");
  Serial.print(F("DHT11-Humidity = "));
  Serial.println(dhtSensor.readHumidity()); //Serial.println(F(" %"));
  
  // DHT11 data from the second sensor
  Serial.print(F("DHT11-Temperature2 = "));
  Serial.println(dhtSensor2.readTemperature());
  Serial.print(F("DHT11-Humidity2 = "));
  Serial.println(dhtSensor2.readHumidity());


  // HCSR04
  // HCSR04 Sensor 1
  hcsr04_1.measureDistance();
  float distance1 = hcsr04_1.getDistance();
  if (distance1 != -1) {
      Serial.print("HCSR04-1 Distance = ");
      Serial.println(distance1); // Serial.println(" cm");
  } else {
      Serial.println("HCSR04-1 Distance: Out of range");
  }

  // HCSR04 Sensor 2
  hcsr04_2.measureDistance();
  float distance2 = hcsr04_2.getDistance();
  if (distance2 != -1) {
      Serial.print("HCSR04-2 Distance = ");
      Serial.println(distance2); // Serial.println(" cm");
  } else {
      Serial.println("HCSR04-2 Distance: Out of range");
  }

  // HCSR04 Sensor 3
  hcsr04_3.measureDistance();
  float distance3 = hcsr04_3.getDistance();
  if (distance3 != -1) {
      Serial.print("HCSR04-3 Distance = ");
      Serial.println(distance3); // Serial.println(" cm");
  } else {
      Serial.println("HCSR04-3 Distance: Out of range");
  }

  // HCSR04 Sensor 4
  hcsr04_4.measureDistance();
  float distance4 = hcsr04_4.getDistance();
  if (distance4 != -1) {
      Serial.print("HCSR04-4 Distance = ");
      Serial.println (distance4); //Serial.println(" cm");
  } else {
      Serial.println("HCSR04-4 Distance: Out of range");
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
    Serial.print("DS18B20-Water-Temperature: ");
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

  // SGP30 Air Quality Measurement
  tcaSelect(SGP30_CHANNEL); // Ensure SGP30 channel is selected before reading
  if (!sgp.IAQmeasure()) {
    Serial.println("Failed to perform SGP30 IAQ measurement");
  } else {
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

} // END sendSensorDataToRaspberryPi()


