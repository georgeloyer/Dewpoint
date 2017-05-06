/*******************************************************
 * Interrupt Test for Feather M0 and Touch Screen
 * 4/23/2017 
 * Written by George Loyer
 * BSD license
 * This is a test program to validate the operation of external interrupts
 * with the touch screen on the TFT Feather Wing.
 *******************************************************/

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

const byte interruptPin = 12;
volatile uint16_t x, y;
volatile uint8_t z;
volatile boolean pointFlag = false;
boolean buttonFlag = true;

// Interrupt Service Routine (ISR)
void touchEvent() {
  TS_Point p = ts.getPoint();
  x = p.x;
  y = p.y;
  z = p.z;
  pointFlag = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Touch Screen Interrupt Test");

  Serial.println("Starting touchscreen controller.");
  
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller.");
    while (1);
  }
  Serial.println("Touchscreen started.");

  Serial.println("Starting TFT device.");
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  Serial.println("Yielding.");
  yield();
  Serial.println("Started Touch Screen and Screen Driver.");
  tft.fillRoundRect(100,100,120,40,4,ILI9341_RED);

  Serial.print("digitalPinToInterrupt(12) is ");
  Serial.println(digitalPinToInterrupt(interruptPin));
  attachInterrupt(digitalPinToInterrupt(interruptPin), touchEvent, HIGH);
  
}

void loop() {
  if (pointFlag) {
    Serial.print("x=");
    Serial.print(x);
    //Serial.print(map(x, TS_MINX, TS_MAXX, 0, tft.width()));
    Serial.print("   y=");
    Serial.print(y);
    //Serial.println(map(y, TS_MINY, TS_MAXY, 0, tft.height()));
    Serial.print("  z=");
    Serial.println(z);
    pointFlag = false;
    if (buttonFlag) {
      tft.fillRoundRect(100,100,120,40,4,ILI9341_BLUE);
    } else {
      tft.fillRoundRect(100,100,120,40,4,ILI9341_RED);
    }
    buttonFlag = !buttonFlag;
  }
}
