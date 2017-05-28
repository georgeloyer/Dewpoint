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

boolean writeToLogFile(String logLine) {
  SdFile logFile;

  if (!logFile.open(fullPathLogFile.c_str(), O_CREAT | O_APPEND | O_RDWR)) {
    if (DEBUG) {
      String errString = "ERROR: writeToLogFile: Failed to open log file: " + fullPathLogFile;
      Serial.println(errString);
    }
    return false;                            // return false if write to log file fails
  }

  if (logFile.fileSize() > MAXLOGFILE) {              // roll the log file if it's greater than 100KB in size
    logFile.close();                              // close the file that is greater than 100KB in size
    if (!rollLogFile()) {
      if (DEBUG) {
        Serial.println("ERROR: writeToLogFile: Log file roll failed to return a valid log file name and path.");
      }
      return false;                          // return false if write to log file fails      
    } else {
      // open the new log file
      if (!logFile.open(fullPathLogFile.c_str(), O_CREAT | O_APPEND | O_RDWR)) {
        if (DEBUG) {
          String errString = "ERROR: writeToLogFile: Failed to open NEW log file: " + fullPathLogFile;
          Serial.println(errString);
        }
        return false;                            // return false if write to log file fails
      }
    }
  }

  if (int numbytes = logFile.write(logLine.c_str()) == -1) {
    if (DEBUG) {
      String errString = "ERROR: writeToLogFile: Failed to write to log file, error -1: " + fullPathLogFile + ". Data: " + logLine;
      Serial.println(errString);
    }
    return false;                                  // return false if write to log file fails
  } else {
    if (logLine.length() != numbytes) {
      if (DEBUG) {
        String errString = "ERROR: writeToLogFile: Failed to write to log file, error wrong bytes: " + fullPathLogFile + ". Data: " + logLine;
        Serial.println(errString);
        errString = String("ERROR: Wrote ") + String(numbytes) + String(", expected ") + String(logLine.length()) + String(" bytes written.");
        Serial.println(errString);
      }
      return false;                                // return false if write to log file fails.
    }
  }

  logFile.close();

  return true;
}

