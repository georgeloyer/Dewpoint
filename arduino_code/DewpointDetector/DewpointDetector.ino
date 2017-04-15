/*******************************************************
 * DewpointDetector
 * 4/9/2017 
 * Written by George Loyer
 * BSD license
 * This is a DewpointDetector device intended for use on
 * the RFO 40-inch telescope to monitor and prevent dew
 * from forming on the primary mirror of the telescope.
 * 
 * The dewpoint is a function of air temperature, humidity
 * and the temperature of the object that will allow dew
 * to form upon it when the dew threshold is reached.
 * 
 * In addition to sensing these three values, the unit
 * also logs to an SD card, connects via WiFi to an IOT
 * system and reports periodic readings to the IOT 
 * repository, which also provides a web interface for
 * showing a timeline of readings.
 * 
 * Finally, this unit includes a touchscreen as a user
 * interface for reading out the current state, showing
 * recent trends, and controlling the operation of the 
 * unit, including display of the three measures in 
 * real time.
 *******************************************************/

#include "DHT.h"
#define DHTPIN 10     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// Connect pin 1 (on the left) of the sensor to +3.3V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#include <Wire.h>
#include "Adafruit_TMP007.h"
// Connect VCC to +3.3V 
// Gnd -> Gnd
// SCL connects to the I2C clock pin.
// SDA connects to the I2C data pin. 

#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <SD.h>
#include <Adafruit_STMPE610.h>

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef __AVR_ATmega32U4__
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef ARDUINO_SAMD_FEATHER_M0
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);


// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

// Color definitions
#define BLACK    0x0000
#define GREEN    0x001F 
#define BLUE     0xF800 
#define RED      0x07E0 
#define YELLOW   0x07FF 
#define CYAN     0xF81F 
#define MAGENTA  0xFFE0 
#define WHITE    0xFFFF

#define PENRADIUS 3

DHT dht(DHTPIN, DHTTYPE);
Adafruit_TMP007 tmp007; //Start with the default i2c address 0x40

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("DewpointDetector 1.0");

  Serial.println("Starting touchscreen controller.");
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller.");
    while (1);
  }
  Serial.println("Touchscreen started.");

  Serial.println("Starting TFT device.");
  tft.begin();
  tft.fillScreen(ILI9341_RED);
  tft.setRotation(3);
  tft.fillRect(10, 10, 300, 220, BLACK);


  Serial.println("Yielding.");
  yield();

  Serial.println("Started Touch Screen and Screen Driver.");
  Serial.print("Starting SD card.");
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card start failed!");
  }
  Serial.println("SD cart started.");

  // you can also use tmp007.begin(TMP007_CFG_1SAMPLE) or 2SAMPLE/4SAMPLE/8SAMPLE to have
  // lower precision, higher rate sampling. default is TMP007_CFG_16SAMPLE which takes
  // 4 seconds per reading (16 samples)
  Serial.println("Starting thermopile tmp007 sensor.");
  if (! tmp007.begin()) {
    Serial.println("No thermopile sensor found");
    while (1);
  }
  Serial.println("Thermopile tmp007 sensor started.");

  Serial.println("Starting Humidity-temp sensor DHT-11.");
  dht.begin(); 

}

void loop() {
   float objt = tmp007.readObjTempC();
   Serial.print("Object Temperature: "); Serial.print(objt); Serial.println("*C");
   float diet = tmp007.readDieTempC();
   Serial.print("Die Temperature: "); Serial.print(diet); Serial.println("*C");
   
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  float d = dewpointC(t, h);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Dewpoint temp: ");
  Serial.print(d);
  Serial.print(" *C ");
  Serial.print(CtoF(d));
  Serial.println(" *F");
//  Serial.print("Heat index: ");
//  Serial.print(hic);
//  Serial.print(" *C ");
//  Serial.print(hif);
//  Serial.println(" *F");

  delay(4000); // 4 seconds per reading for 16 samples per reading, and give DHT sensor a rest, too

}

float FtoC(float f) {
  float c;

  c = (f - 32.0) * 5.0 / 9.0;
  return c;
}
float CtoF(float c) {
  float f;

  f = c * 9.0 / 5.0 + 32.0;
  return f;
}
float CtoK(float c) {
  float k;

  k = c + 273.15;
  return k;
}
float KtoC(float k) {
  float c;

  c = k - 273.15;
  return c;
}
float dewpointC(float c, float h) {
  float d, es, e;

  es = 0.611*exp(5423.0*(1.0/273.15 - 1.0/(c-273.15)));
  e = h/100.0*es;
  d = (5423.0/(19.854 - log(e/0.611))) + 273.15;
  return d;
}

