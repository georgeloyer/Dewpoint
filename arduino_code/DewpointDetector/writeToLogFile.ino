/*
 * writeToLogFile(DewLog dewlog)
 * History: 
 * 2017-05    gel Original
 * 2017-05-17 gel TODO: consider revising population of log line from dewlog struct to include data validation for specific
 *                values in the data set. This could also be done in the method calling.
 *                TODO: consider an alternative error return code (second return value) for the multiple causes of a log write failure.
 * 
 * Opens log file, returns null string if it fails.
 * Checks log file size, if it's greater than 100KB, closes it and opens a new one (using rollLogFile())
 * Creates the log line
 * Writes the log line to the log file
 * Returns the current log file name and path
 */

String writeToLogFile(DewLog dewlog) {
  SdFile logFile;
  String logLine;

  if (!logFile.open(dewlog.fullPathLogFile.c_str(), O_CREAT | O_APPEND | O_RDWR)) {
    if (DEBUG) {
      String errString = "Error: Failed to open log file: " + dewlog.fullPathLogFile;
      Serial.println(errString);
    }
    return String("");                            // return null string if write to log file fails
  }

  if (logFile.fileSize() > 102400) {              // roll the log file if it's greater than 100KB in size
    logFile.close();                              // close the file that is greater than 100KB in size
    String newLogFile = rollLogFile(dewlog.fullPathLogFile);
    if (newLogFile == "") {
      if (DEBUG) {
        Serial.println("Log file roll failed to return a valid log file name and path.");
      }
      return newLogFile;                          // return null string if write to log file fails
    }
    dewlog.fullPathLogFile = newLogFile;

    // open the new log file
      if (DEBUG) {
        String errString = "Error: Failed to open NEW log file: " + dewlog.fullPathLogFile;
        Serial.println(errString);
      }
      return String("");                            // return null string if write to log file fails
    }

  // create the log line
  logLine = dewlog.dateTime + "\t";
  logLine += String(dewlog.humidity, 2) + "\t";
  logLine += String(dewlog.temperature, 2) + "\t";
  logLine += String(dewlog.mirrorTemp, 2) + "\t";
  logLine += String(dewlog.dewpointTemp, 2) + "\t";
  if (dewlog.heaterStatus) {
    logLine += String("Heater ON") + "\r";
  } else {
    logLine += String("Heater OFF") + "\r";
  }

  if (int numbytes = logFile.write(logLine.c_str()) == -1) {
    if (DEBUG) {
      String errString = "Error: Failed to write to log file, error -1: " + dewlog.fullPathLogFile + ". Data: " + logLine;
      Serial.println(errString);
    }
    return String("");                                  // return null string if write to log file fails
  } else {
    if (logLine.length() != numbytes) {
      if (DEBUG) {
        String errString = "Error: Failed to write to log file, error wrong bytes: " + dewlog.fullPathLogFile + ". Data: " + logLine;
        Serial.println(errString);
        errString = String("Error: Wrote ") + String(numbytes) + String(", expected ") + String(logLine.length()) + String(" bytes written.");
        Serial.println(errString);
      }
      return String("");                                // return null string if write to log file fails.
    }
  }

  logFile.close();

  return dewlog.fullPathLogFile;
}

