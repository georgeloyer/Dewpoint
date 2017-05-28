/*
 * setupLogFile()
 * History: 
 *  2017-05 gel Original version
 *  2017-05-16 gel  TODO: revise to include current.txt file in /datalog folder containing the path of the current log file, 
 *                  initialized by setupLogFile() and updated by rollLogFile(). Handle edge cases (check for evidence that 
 *                  the value stored in current.txt is not accurate).
 *  
 * returns a string with the full path of the log file, including the name
 * 
 * Assumes the starting pathname is at the root, /datalog.
 * Traverses the directories inside of /datalog, if any exist, to find the latest one.
 * All directories in /datalog have a name with the pattern dlogNNN where NNN is an integer.
 * If there is no dlogNN folder in /datalog, create dlog001.
 * Within the latest folder, search for the last file written. The files have the pattern 
 * dewptNN.log where NN is an integer. If no dewpt*.log file exists, then return as the log
 * file name, dewpt01.log. The next user of the log file will open it with O_CREATE option.
 * setupLogFile assumes that the progam may have been running and producing log files in the past.
 * 
 */
String setupLogFile() {
  SdFile myFile;
  char tempFileName[13];
  String fileName;

  fullPathLogFile = "datalog";
  if (!sd.exists("datalog")) {
    if (!sd.mkdir(fullPathLogFile.c_str(), true)) {       // doesn't exist? try to create it and its parent directories
      if (DEBUG) {                                        // failed to create the top folder, give up
        Serial.print("ERROR: cannot create ");
        Serial.print(fullPathLogFile.c_str());
        Serial.println(" folder on SD card");
      }
      return String("");                              // on failure to create datalog folder, return null String
    } else {
      if (DEBUG) {
        Serial.println("INFO: Created /datalog folder");
      }
    }
  }

  fullPathLogFile = "/" + fullPathLogFile;
  if (!sd.chdir(fullPathLogFile.c_str())) {
    if (DEBUG) {
      Serial.print("ERROR: failed to chdir to ");
      Serial.println(fullPathLogFile.c_str());
    }
    return String("");
  }

  sd.vwd()->rewind();
  fileName = "";                                              // set filename to null
  while (myFile.openNext(sd.vwd(), O_READ)) {
    if (myFile.isDir()) {
      myFile.getSFN(tempFileName);
      if (String(tempFileName).substring(0,4) == "dlog") {
        fileName = String(tempFileName);
      }
    }
    myFile.close();
  }
  
  if (fileName == "") {                                       // we didn't find a dlogNNN file, so we create the first one
    fileName = "dlog001";
    if (!sd.mkdir(fileName.c_str())) {                         // create the path
      if (DEBUG) {
        Serial.print("ERROR: cannot create ");
        Serial.print(fileName.c_str());
        Serial.print(" folder in ");
        Serial.println(fullPathLogFile.c_str());
      }
      return String("");
    }
  }

  fullPathLogFile += "/" + fileName;
  if (!sd.chdir(fullPathLogFile.c_str())) {
    if (DEBUG) {
      Serial.print("ERROR: unable to set working directory to ");
      Serial.println(fullPathLogFile.c_str());
    }
    return String("");
  }

  sd.vwd()->rewind();
  fileName = "";
  while (myFile.openNext(sd.vwd(), O_READ)) {
    if (!myFile.isDir()) {
      myFile.getSFN(tempFileName);
      if (String(tempFileName).substring(0,5) == "dewpt") {
        fileName = String(tempFileName);
      }
    }
    myFile.close();
  }

  if (fileName == "") {                                 // and we didn't find one, set the filename to the first log file in this folder
    fileName = "dewpt01.log";
  }

  fullPathLogFile +=  "/" + fileName;

  if (!myFile.open(fullPathLogFile.c_str(), FILE_WRITE)) {
    if (DEBUG) {
      Serial.print("ERROR: cannot open log file ");
      Serial.println(fullPathLogFile);
    }
    return String("");
  } else {
    myFile.close();
  }

  return fullPathLogFile;
}

