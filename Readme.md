

**********************************************************************
// Quick start Guide for code architecture
// 2024-03-01 Version 1.1
// Jason Tomkins

**********************************************************************
<h3>Sample Output:</h3>

DHT11-Temperature = 19.40<br>
DHT11-Humidity = 21.00<br>
DHT11-Temperature2 = 21.2<br>
DHT11-Humidity2 = 19.0<br>
HCSR04-1-Distance = 26.47<br>
HCSR04-2-Distance: 30.0<br>
HCSR04-3-Distance: 42.0<br>
HCSR04-4-Distance: 18.0<br>
KY018-1-Voltage = 948.19<br>
KY018-1-Resistance = 2340.17<br>
KY018-1-Light-Intensity = 91.86<br>
KY018-2-Voltage = 992.18<br>
KY018-2-Resistance = 2475.61<br>
KY018-2-Light-Intensity = 86.83<br>
KY018-3-Voltage = 860.22<br>
KY018-3-Resistance = 2077.92<br>
KY018-3-Light-Intensity = 103.45<br>
KY018-4-Voltage = 1432.06<br>
KY018-4-Resistance = 4013.70<br>
KY018-4-Light-Intensity = 53.56<br>
KY018-5-Voltage = 1432.06<br>
KY018-5-Resistance = 4032.92<br>
KY018-5-Light-Intensity = 53.30<br>
DS18B20-Water-Temperature = 23.0<br>
KS0429-TotalDissolvedSolids = 640<br>
CJMCU6814-Nitrogen-Dioxide(NO2) = -19.40<br>
CJMCU6814-Amonia(NH3) = 2.10<br>
CJMCU6814-Carbon-Monoxide(CO) = 1.60<br>
SGP30-TVOC = 1271<br>
SGP30-eCO2 = 1841<br>
BMP280-Temperature = 20.15<br>
BMP280-Pressure = 1015.57<br>
BMP280-Altitude = 110.71<br>
RH4502C-pH = 21.51<br>

**********************************************************************
Settings available for: Serial Plotter Output frequency(time): 1min or 10min or 60min. uncomment "//" the row in Main.ino as seen below.

// ADJST YOUR TIME in Main.ino
unsigned long lastSensorReadTime = 0;<br>
const unsigned long sensorReadInterval = 60000; // 1min<br>
//const unsigned long sensorReadInterval = 600000; // 10min<br>
//const unsigned long sensorReadInterval = 3600000; // 60min<br>

**********************************************************************


<h3>The Code is modular:</h3>
All sensor code follow this directory structure:<br>
	Main.ino
  <ul>>
	<li>#include and #define statements</li>
	<li>setup up objects</li>
	<li>loop objects</li>
	<li>Important function: "sendSensorDataToRaspberryPi();" Print Data output to serial and send to Raspberry pi</li>
	<li>a few instantiations for objects</li>
	<li>sensorName.cpp script</li>
	<li>sensorName.h script</li>
  </ul>


**********************************************************************

<h2>10 Unique Sensors</h2)
<h2>33 Data Points</h2)

<h3>Physical Archtecture:</h3>
2 Breadboards:<br>
<ul>
<li>Full Size Breadboard: 5 Volt double-rail</li>
<li>Mid-Size Breadboard: 3.3 Volt double-rail</li>
</ul>
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
<h2>ANALOGUE</h2>
*****************************************************

N02, NH3 and CO Sensor<br>
CJMCU6814 - 5Volt - Analogue Pin# A0, A1, A2<br>

pH Sensor<br>
RH4502 - 5Volt - Analogue Pin #A5<br>

TDS Sensor<br>
KS0429 - 3.3Volt - Analogue Pin #A7<br>

Light/Resitance/Voltage<br>
<ul>
<li>KY018 - 5Volt - Analogue Pin #A8</li><br>
<li>KY018 - 5Volt - Analogue Pin #A9</li><br>
<li>KY018 - 5Volt - Analogue Pin #A10</li><br>
<li>KY018 - 5Volt - Analogue Pin #A11</li><br>
<li>KY018 - 5Volt - Analogue Pin #A12</li><br>
</ul>

*****************************************************
<h2>DIGITAL</h2>
*****************************************************

Temp/Humidity
DHT11 - 3.3Volt - Digital Pin #2<br>
DHT11 - 3.3Volt - Digital Pin #3

Distance
<ul>
<li>HCSR04 - 5Volt - Digital Pin# 12, 13</li>
<li>HCSR04 - 5Volt - Digital Pin# 10, 11</li>
<li>HCSR04 - 5Volt - Digital Pin# 8, 9</li>
<li>HCSR04 - 5Volt - Digital Pin# 6, 7</li>
</ul>

Water Temp
DS18B20 - 5Volt - Digital Pin# 5<br>

Multiplex - 3.3Volt - SDA/SCL<br>
VOC and eC02 Sensor
<ul>
<li>SGP30 - SDA0
<li>SGP30 - SCL0
</ul>

Temp, Pressure, Altitude Sensor <br>
<ul>
<li>BMP280 - SDA1</li>
<li>BMP280 - SCL1</li>
</ul>

**********************************************************************
CALIBRATION SCRIPTS
**********************************************************************

For the RH-4502C Ph Meter
// Calibration: Wrap copper around the screw head, plug into 5v VCC and stick a copper wite inside the center/circular sensor.<br>
// STEP 1 - START CALIBRATION. If VCC = 5.0 Volt, or near VCC = 5V good result.<br>
// Code BELOW:<br>


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
 <br>
// END CALIBRATION SCRIPT

**********************************************************************

<h2>Disclaimer:</h2>

A lot of calibration is underway. All sensors are pulling or reading data but the scales and code verification required.

**********************************************************************


