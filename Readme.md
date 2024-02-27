

**********************************************************************
// Quick start Guide for code architecture
// 2024-02-27 Version 1.0
// Jason Tomkins
**********************************************************************

Settings available for: Serial Plotter Output frequency(time): 1min or 10min or 60min. uncomment "//" the row in Main.ino as seen below.

// ADJST YOUR TIME in Main.ino
unsigned long lastSensorReadTime = 0;
const unsigned long sensorReadInterval = 60000; // 1min
//const unsigned long sensorReadInterval = 600000; // 10min
//const unsigned long sensorReadInterval = 3600000; // 60min


**********************************************************************
Sample Output (KY018 Repeats 5times for measuring light):

DHT11-Temperature = 21.80
DHT11-Humidity = 24.00
HCSR04-Distance = 9.84
KY018-1-Voltage = 957.97
KY018-1-Resistance = 2370.01
KY018-1-Light Intensity = 98.11
KY018-2-Voltage = 977.52
KY018-2-Resistance = 2430.13
KY018-2-Light Intensity = 95.69
KY018-3-Voltage = 972.63
KY018-3-Resistance = 2415.05
KY018-3-Light Intensity = 96.28
KY018-4-Voltage = 1524.93
KY018-4-Resistance = 4428.77
KY018-4-Light Intensity = 52.50
KY018-5-Voltage = 1505.38
KY018-5-Resistance = 4287.71
KY018-5-Light Intensity = 54.23
DS18B20-Temperature: 21.00
KS0429-TotalDissolvedSolids = 7103
CJMCU6814-Nitrogen-Dioxide(NO2) = 1.00
CJMCU6814-Amonia(NH3) = 2.10
CJMCU6814-Carbon-Monoxide(CO) = 1.70
SGP30-TVOC = 1267
SGP30-eCO2 = 1837
BMP280-Temperature = 22.28
BMP280-Pressure = 991.34
BMP280-Altitude = 313.44
RH4502C-pH = 7.19

**********************************************************************

The Code is modular:
All sensor code follow this directory structure:
	Main.ino
		#include and #define statements
		setup up objects
		loop objects
		Import function: "sendSensorDataToRaspberryPi();" Print Data output and send to Raspberry pi
		a few instantiations for objects
	sensorName.cpp script
	sensorName.h script


**********************************************************************

10 Unique Sensors
29 Data Points

Physical Archtecture:
2 Breadboards:
	Full Size Breadboard: 5 Volt double-rail
	Mid-Size Breadboard: 3.3 Volt double-rail

Arduino Megas supports pins for both 5.0V and 3.3 Volt.

Sensor |CONNECT| toBreadboard |CONNECT| toArduino |CONNECT| Raspberry Pi4

Raspberry Pi 4 details:
	5 inch Touchscreen from Waveshare
	Thoney Script receiving data from Arduino via USB Serial (Arduino plugs into the Pi via USB).
	python script running in Thony pushes the data to Influx.db
	Graphana reads to the data from influx.db displayed on the 5inch touchscreen

**********************************************************************
	
Interesting Sensors for better accuracy:
	The SGP30 inherits the DHTT Humidity value for better accuracy (sensors benefiting each other)
		SGP30-eC02: equivalent Carbon Dioxide concentration
		SGP30-TVOC: Total Volatile Organic Compounds in the air
	The BMP280 inerhiets the DHT11 Temperature value for better accuracy for 1 calculation
	The BMP280 requires a value to determine approx. altitude "meters above sea-level" for your location.
	
	
		

**********************************************************************

Disclaimer:

A lot of calibration is underway. All sensors are pulling or reading data but the scales and code verification required.

**********************************************************************





**********************************************************************
CALIBRATION SCRIPTS
**********************************************************************

For the RH-4502C Ph Meter
// Calibration: Wrap copper around the screw head, plug into 5v VCC and stick a copper wite inside the center/circular sensor.
// STEP 1 - START CALIBRATION. If VCC = 5.0 Volt, or near VCC = 5V good result.
// Code BELOW:


int pH_Value; 
float Voltage;

void setup() 
  { 
    Serial.begin(9600);
    pinMode(pH_Value, INPUT); 
 } 
 
void loop() 
 { 
   pH_Value = analogRead(A7); <-- Change to your Arduino Analogue PIN
   Voltage = pH_Value * (5.0 / 1023.0); 
   Serial.println(Voltage); 
   delay(500); 
 }
// END CALIBRATION SCRIPT

**********************************************************************












**********************************************************************
