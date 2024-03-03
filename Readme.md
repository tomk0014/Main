

**********************************************************************
// Quick start Guide for code architecture
// 2024-03-01 Version 1.1
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

DHT11-Temperature = 19.40
DHT11-Humidity = 21.00
DHT11-Temperature2 = 21.2
DHT11-Humidity2 = 19.0
HCSR04-1-Distance = 26.47
HCSR04-2-Distance: 30.0
HCSR04-3-Distance: 42.0
HCSR04-4-Distance: 18.0
KY018-1-Voltage = 948.19
KY018-1-Resistance = 2340.17
KY018-1-Light-Intensity = 91.86
KY018-2-Voltage = 992.18
KY018-2-Resistance = 2475.61
KY018-2-Light-Intensity = 86.83
KY018-3-Voltage = 860.22
KY018-3-Resistance = 2077.92
KY018-3-Light-Intensity = 103.45
KY018-4-Voltage = 1432.06
KY018-4-Resistance = 4013.70
KY018-4-Light-Intensity = 53.56
KY018-5-Voltage = 1432.06
KY018-5-Resistance = 4032.92
KY018-5-Light-Intensity = 53.30
DS18B20-Water-Temperature = 23.0 
KS0429-TotalDissolvedSolids = 640
CJMCU6814-Nitrogen-Dioxide(NO2) = -19.40
CJMCU6814-Amonia(NH3) = 2.10
CJMCU6814-Carbon-Monoxide(CO) = 1.60
SGP30-TVOC = 1271
SGP30-eCO2 = 1841
BMP280-Temperature = 20.15
BMP280-Pressure = 1015.57
BMP280-Altitude = 110.71
RH4502C-pH = 21.51

**********************************************************************

The Code is modular:
All sensor code follow this directory structure:
	Main.ino
		#include and #define statements
		setup up objects
		loop objects
		Important function: "sendSensorDataToRaspberryPi();" Print Data output to serial and send to Raspberry pi
		a few instantiations for objects
	sensorName.cpp script
	sensorName.h script


**********************************************************************

10 Unique Sensors
33 Data Points

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
Arduino Mega Pin Configuration for Sensors.


*****************************************************
ANALOGUE
*****************************************************

N02, NH3 and CO
CJMCU6814 - Analogue Pin# A0, A1, A2

Ph Sensor
RH-4502 - Analogue Pin #A5

TDS Sensor
KS0429 - Analogue Pin #A7

Light/Resitance/Voltage
KY018 - Analogue Pin #A8
KY018 - Analogue Pin #A9
KY018 - Analogue Pin #A10
KY018 - Analogue Pin #A11
KY018 - Analogue Pin #A12


*****************************************************
DIGITAL
*****************************************************

Temp/Humidity
DHT11 - Digital Pin #2
DHT11 - Digital Pin #3

Distance
HCSR04 - Digital Pin# 12, 13
HCSR04 - Digital Pin# 10, 11
HCSR04 - Digital Pin# 8, 9
HCSR04 - Digital Pin# 6, 7

Water Temp
DS18B20 - Digital Pin# 5

Multiplex: SDA/SCL
	VOC, eC02
	SGP30 - SDA0
	SGP30 - SCL0

	Temp, Pressure, Altitude
	BMP280 - SDA1
	BMP280 - SCL1


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

Disclaimer:

A lot of calibration is underway. All sensors are pulling or reading data but the scales and code verification required.

**********************************************************************


