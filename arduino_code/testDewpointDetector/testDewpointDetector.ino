/*
 * testDewpointDetector
 * 
 * main program for running tests of all functions/methods related to DewpointDetector
 * Includes tests for:
 * 1. float CtoF(float) - compute a range of conversions and compare to manually calculated values
 * 2. float dewpointC(float t, float h) - compute a crosstab of H vs T dewpoint values and compare to manually calculated values
 * 3. boolean rollLogFile() - 
 * 3a. check correct rotation of log files through 99 per folder and through 999 folders
 *      validate single, double and triple digit numeric folder and file numbers are correctly zero-prefixed
 *      validate new folders are created
 *      validate correct creation of new folders and new files on overflow of folder and file numbers
 *      validate correct handling of rollover from folder 999 and file 99
 *      validate timestamps are correct
 * 4. boolean sdFreeSpace() 
 * 4a. write to the SD card and fill to > 40% full and then increment by 5% each iteration
 * 4b. validate correct result in return code until percent full is 95%
 * 4c. clear the disk drive
 * 5. boolean setupLogFile() 
 *      validate folders and first file are created when disk starts empty
 *      validate next file name and path is correct for these cases
 *        5 folders, 67 files in last folder
 *        21 folders, 99 files in last folder
 *        167 folders, 3 log files in last folder
 *        989 folders, 44 log files in last folder
 *      record the time of each run and log
 * 6. boolean writeToLogFile(String)
 *      Validate failure to open log file
 *      Validate write to log file if it is less than limit (100KB)
 *      Validate rollLogFile is called if file size is>= 100KB
 *      Validate failure to open new log file
 *      Validate correct write of data from dewlog struct
 *      Validate failure to write line to log file due to file missing
 *      Validate failure to write full line to log file
 *      
 */

 #include "testDewpointDetector.h"

 void setup() {
  String testLog;
  boolean testSuccess = true;
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Test Suite for DewpointDetector");

  Serial.println("TEST: float CtoF(float)");
  // test float CtoF(float)
  // create canonical data
  float farenheit;
  float ctofCanonical[] = {-4.00, 14.00, 32.00, 50.00, 68.00, 86.00, 104.00, 122.00};
  float ctofInput[] = {-20.0, -10.0, 0.0, 10.0, 20.0, 30.0, 40.0, 50.0};
  for (int i = 0; i < 8; i++) {
    farenheit = CtoF(ctofInput[i]);
    testLog = String("Convert ") + String(ctofInput[i],2) + String("C to ") + String(farenheit,2) + String("F");
    if (abs(farenheit - ctofCanonical[i]) > .001) {
      testLog += String("difference from canonical exceeds 0.001");
      testSuccess = false;
    }
    Serial.println(testLog);
  }
  if (testSuccess) {
    Serial.println("PASSED ctof test");
  } else {
    Serial.println("FAILED ctof test");
  }

  // Next test
  testSuccess = true;
 }

 void loop() {
  
 }

