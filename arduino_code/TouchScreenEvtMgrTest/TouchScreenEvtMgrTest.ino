/*******************************************************
 * Event Manager Test for Feather M0 and Touch Screen
 * 5/1/2017 
 * Written by George Loyer
 * BSD license
 * This is a test program to validate the operation of event handling
 * with the touch screen on the TFT Feather Wing.
 *******************************************************/

#include "EventManager.h"
#define DEBUG 1

// TFT screen and graphics library declarations
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <SD.h>
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

// instantiate instances of each object type
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
EventManager gEM;

boolean buttonFlag = false;
TS_Point p;

// listenerButtonDown
void listenerButtonDown(int eventCode, int eventParam) {
  while (! ts.bufferEmpty()) {    // empty the buffer, take last point
    p = ts.getPoint();     
  }
  downAction1();                  // do the down action
  while (ts.touched()) {          // debounce until up
    delay(25);
  }
  upAction1(buttonFlag);          // do the up action
  gEM.queueEvent(EventManager::kEventUser1, 1);     // queue menu event to act on the touch
}

//listenerMenu
void listenerMenu(int eventCode, int eventParam) {
  return;
  // p.x = map(p.x, 0, 4095, 0, tft.width());
  // p.y = map(p.y, 0, 4095, 0, tft.height());
  // if (p.x > 100 & p.x < 140 & p.y > 100 & p.y < 220) {
}

void setup() {
  if (DEBUG) {
    Serial.begin(115200);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }
  }
  if (DEBUG) {
    Serial.println("Touch Screen Interrupt Test");
    Serial.println("Starting touchscreen controller.");
  }
  if (!ts.begin()) {
    if (DEBUG) {
      Serial.println("Couldn't start touchscreen controller.");
    }
    while (1);
  }
  if (DEBUG) {
    Serial.println("Touchscreen started.");
    Serial.println("Starting TFT device.");
  }
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  yield();
  if (DEBUG) {
    Serial.println("Started Touch Screen and Screen Driver.");
  }
  startAction();
  gEM.addListener(EventManager::kEventUser0, listenerButtonDown); // button down listener
  gEM.addListener(EventManager::kEventUser1, listenerMenu);       // menu listener
  if (DEBUG) {
    Serial.print( "Number of listeners: " );
    Serial.println( gEM.numListeners() );
  }
}

void loop() {
  // Handle any events in the queue
  gEM.processEvent();
  getButtonDownEvent();
}

void getButtonDownEvent() {
  if (!ts.bufferEmpty()) {
    gEM.queueEvent(EventManager::kEventUser0, 1 );    // queue button down event
  } 
}

void startAction() {
  tft.fillRoundRect(100,100,120,40,4,ILI9341_RED);
}

void downAction1() {
  tft.fillRoundRect(100,100,120,40,4,ILI9341_YELLOW);
}

void upAction1(boolean flag) {
  if (flag) {
    tft.fillRoundRect(100,100,120,40,4,ILI9341_RED);
  } else {
    tft.fillRoundRect(100,100,120,40,4,ILI9341_BLUE);
  }
  buttonFlag = !buttonFlag;
}


