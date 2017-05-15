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
// Event Manager declarations
#include "EventManager.h"
#include "dewLog.h"
#define DEBUG 1

// Humidity-Temperature Sensor declarations
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 11     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// Connect pin 1 (on the left) of the sensor to +3.3V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Depends on Adafruit Unified Sensor Library:  https://github.com/adafruit/Adafruit_Sensor
// Depends on DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

// Thermopile sensor declarations
#include <Wire.h>
#include "Adafruit_TMP007.h"
// Connect VCC to +3.3V 
// Gnd -> Gnd
// SCL connects to the I2C clock pin.
// SDA connects to the I2C data pin. 

// WiFi declarations
#include <SPI.h>
#include <WiFi101.h>

// TFT screen and graphics library declarations
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include "SdFat.h"
#include <Adafruit_STMPE610.h>

// CPU macro setup of pins for TFT screen
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

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

#define PENRADIUS 3

// Set USE_SDIO to zero for SPI card access. 
#define USE_SDIO 0
/*
 * SD chip select pin.  Common values are:
 *
 * Arduino Ethernet shield, pin 4.
 * SparkFun SD shield, pin 8.
 * Adafruit SD shields and modules, pin 10.
 * Default SD chip select is the SPI SS pin.
 */
const uint8_t SD_CHIP_SELECT = SD_CS;
#if USE_SDIO
// Use faster SdioCardEX
SdFatSdioEX sd;
// SdFatSdio sd;
#else // USE_SDIO
SdFat sd;
#endif  // USE_SDIO

// instantiate instances of each object type
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
DHT_Unified dht(DHTPIN, DHTTYPE);
Adafruit_TMP007 tmp007;                   //Start with the default i2c address 0x40
EventManager gEM;                         // global for event manager
sensors_event_t event;                    //global for event manager
DewLog dewlog;                            // struct containing dew logging data
float t, h, d, objt, diet;                // globals for sensors: temp, humidity, dewpoint temp, object temp and die temp
unsigned long lastSensorTime, rightNow;   //globals for last time sensors were read and the current time
int sensorTimerSecs;
String fullPathLogFile;                   //global containing full path name of current log file
boolean sdAvailable;                      //global for indicating whether SD card is available
uint32_t cardSize;                        //global for card size

void setup() {
  if (DEBUG) {
    Serial.begin(115200);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }
  }
  if (DEBUG) {
    Serial.println("DewpointDetector 1.0");
  }

  startTouchScreen();                       // starts touch screen, TFT and SD card drivers
  startTMP007();                            // starts thermopile sensor
  startDHT22();                             // starts humidity-temperature sensor
  lastSensorTime = millis();                // time in millis since the last sensor reading
  sensorTimerSecs = 10;                     // time in seconds between sensor readings
  if (sdAvailable = startSdFat()) {         // initialize the SD card
    fullPathLogFile = setupLogFile();       // set the path and name of the log file
  }
  // setup listeners for EventManager
  gEM.addListener(EventManager::kEventUser0, listenerSensorTimer); // Sensor timer listener
  if (DEBUG) {
    Serial.print( "Number of listeners: " );
    Serial.println( gEM.numListeners() );
  }
}

void loop() {
  // Handle any events in the queue
  gEM.processEvent();
  createSensorTimerEvents();
}


/*
  if (DEBUG) {
    Serial.println("Starting WiFi.");
  }
  // Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    if (DEBUG) {
      Serial.println("WiFi shield not present, halting.");
    }
    // don't continue:
    while (true);
  }

  // Print WiFi MAC address:
  printMacAddress();

  // scan for existing networks:
  Serial.println("Scanning available networks...");
  listNetworks();
  //chooseNetwork();
  //requestSend();
*/

