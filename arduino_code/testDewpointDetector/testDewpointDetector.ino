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
  Serial.println("====================================");

  // test float CtoF(float)
  Serial.println("TEST: float CtoF(float)");
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
  Serial.println("====================================");

  // Dewpoint calculator test
  testSuccess = true;
  Serial.println("TEST: float dewpoint(temp, humidity)");
  //create canonical data
  float dpoint;
  float dpHumidity[] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
  float dpTemperature[] = {-10.0, -5.0, 0.0, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0};

  float dpCanonical[][11] = {
                            {-48.69, -42.25, -35.84, -29.45, -23.10, -16.78, -10.49, -4.23, 2.01, 8.21, 14.38},
                            {-35.98, -30.03, -24.10, -18.18, -12.29, -6.42, -0.56, 5.27, 11.09, 16.89, 22.67},
                            {-28.99, -23.31, -17.63, -11.97, -6.33, -0.69, 4.93, 10.53, 16.13, 21.71, 27.27},
                            {-24.23, -18.72, -13.22 , -7.73, -2.24, 3.23, 8.69, 14.14, 19.58, 25.02, 30.44},
                            {-20.63, -15.25, -9.88, -4.52, 0.84, 6.19, 11.54, 16.87, 22.20, 27.52, 32.84},
                            {-17.76, -12.48, -7.21, -1.95, 3.31, 8.57, 13.82, 19.06, 24.30, 29.54, 34.76},
                            {-15.37, -10.18, -5.00, 0.19, 5.37, 10.54, 15.71, 20.88, 26.05, 31.21, 36.37},
                            {-13.34, -8.22, -3.10, 2.01, 7.12, 12.23, 17.34, 22.44, 27.54, 32.64, 37.74},
                            {-11.57, -6.51, -1.46, 3.60, 8.65, 13.70, 18.75, 23.80, 28.85, 33.89, 38.94},
                            {-10.00, -5.00, 0.00, 5.00, 10.00, 15.00, 20.00, 25.00, 30.00, 35.00, 40.00}
                            };
  for (int h=0; h<10; h++) {
    for (int t=0; t<11; t++) {
      dpoint = dewpointC(dpTemperature[t], dpHumidity[h]);
      testLog = String("Dewpoint for temperature ") + String(dpTemperature[t],2) + String("C and Humidity ") + String(dpHumidity[h],2) + String("% is ") + String(dpoint);
      if (abs(dpoint - dpCanonical[h][t]) > .02) {
        testLog += String("  FAIL: ") + String(abs(dpoint - dpCanonical[h][t]), 4) + String(" difference from canonical exceeds 0.020");
        testSuccess = false;
      }
      Serial.println(testLog);
    }
  }
  if (testSuccess) {
    Serial.println("PASSED dewpointC test");
  } else {
    Serial.println("FAILED dewpointC test");
  }
  Serial.println("====================================");
}

 void loop() {
  
 }

