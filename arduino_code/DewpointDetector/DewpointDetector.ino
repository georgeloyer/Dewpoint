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
// INCLUDES
#include <SPI.h>                          // used by Wifi library
#include "EventManager.h"                 // Event Manager library
#include <DHT_U.h>                        // DHT unified sensor library (both must be included)
#include "Adafruit_TMP007.h"              // TMP007 sensor library
#include <WiFi101.h>                      // Wifi library
#include <Adafruit_ILI9341.h>             // TFT Hardware-specific library
#include <Adafruit_STMPE610.h>            // TFT Touch Screen library
#include "SdFat.h"                        // SD card SdFat library

// DEFINEs
#define DEBUG 1                           // turn Serial debug statements on
#define DHTPIN 11                         // what digital pin we're connected to
#define DHTTYPE DHT22                     // DHT 22  (AM2302), AM2321

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
#define MAXLOGFILE 102400                 // max log file size 100KB

// INSTANTIATE instances of each object type
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
DHT_Unified dht(DHTPIN, DHTTYPE);
Adafruit_TMP007 tmp007;                   //Start with the default i2c address 0x40
const uint8_t SD_CHIP_SELECT = SD_CS;     // SD chip select pin.  
SdFat sd;                                 // SdFat device
EventManager gEM;                         // global for event manager
sensors_event_t event;                    // global for event manager
float t, h, d, objt, diet;                // globals for sensors: temp, humidity, dewpoint temp, object temp and die temp
unsigned long rightNow;                   // global for the current time
unsigned long nextSensorTime;             // global for next sensor time
int sensorTimerSecs = 10;                 // global time between sensor readings
boolean heaterStatus = false;             // global status of heater
boolean rolled = false;                   // global boolean for rolled status
String fullPathLogFile;                   // global containing full path name of current log file
boolean sdAvailable;                      // global for indicating whether SD card is available
uint32_t cardSize;                        // global for card size

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

  startTouchScreen();                       // starts touch screen, TFT 
  drawAppScreen();
  startTMP007();                            // starts thermopile sensor
  startDHT22();                             // starts humidity-temperature sensor
  nextSensorTime = millis() + sensorTimerSecs*1000;                
                                            // time in millis until the next sensor reading
  if (sdAvailable = startSdFat()) {         // initialize the SD card
    if (!setupLogFile()) {                  // set the path and name of the log file
      sdAvailable = false;                  // error encountered during setupLogFile - turn sdAvailable false
      if (DEBUG) {
        Serial.println("ERROR: setup: failure during setupLogFile, no logging will be done");
      }
    }
  } else {
    if (DEBUG) {
      Serial.println("ERROR: setup: SD card initialization failed. No logging will be done");
    }
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

// SENSOR COMMENTS
// DHT22
// Connect pin 1 (on the left) of the sensor to +3.3V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Depends on Adafruit Unified Sensor Library:  https://github.com/adafruit/Adafruit_Sensor
// Depends on DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

// TMP007 - Thermopile sensor declarations
// Connect VCC to +3.3V 
// Gnd -> Gnd
// SCL connects to the I2C clock pin.
// SDA connects to the I2C data pin. 



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

