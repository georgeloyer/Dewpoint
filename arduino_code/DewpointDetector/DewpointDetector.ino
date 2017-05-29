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
#include "DewpointDetector.h"

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

