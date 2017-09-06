/* 
 *  DewpointDetector.h
 *  
 *  Includes for all libraries used in the device
 *  Defines for all macros used in the program
 *  Global variable declarations
 */

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

char ssid[] = "NETGEAR31"; //  your network SSID (name)
char pass[] = "ayodele7";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


