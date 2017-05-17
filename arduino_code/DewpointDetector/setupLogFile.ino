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
  SdFile file;
  char tempFileName[13];
  String fileName;
  
  fullPathLogFile = "/datalog";
  if (!sd.chdir(fullPathLogFile.c_str(), true)) {       // make /datalog the working directory, create it if necessary
    if (!sd.mkdir(fullPathLogFile.c_str(), true)) {
      if (DEBUG) {
        Serial.println("Error: cannot create /datalog folder on SD card");
      }
      return String("");                              // on failure to create datalog folder, return null String
    } else {
      sd.chdir(fullPathLogFile.c_str(), true);
    }
  }

  sd.vwd()->rewind();                                 // find the last dlogNNN folder in /datalog (cwd)
  fileName = "";
  while (file.openNext(sd.vwd(), O_READ)) {
    if (file.isDir()) {
      file.getSFN(tempFileName);
      if (String(tempFileName).substring(1,4) == "dlog") {
        fileName = String(tempFileName);
      }
    }
    file.close();
  }
  
  if (fileName == "") {
    fileName = "dlog001";
    if (!sd.mkdir(fileName.c_str(), true)) {
      if (DEBUG) {
        Serial.println("Error: cannot create /datalog/dlogNN folder.");
      }
      return String("");
    }
  }

  sd.chdir(fileName.c_str(), true);
  fullPathLogFile = fullPathLogFile + "/" + fileName;

  sd.vwd()->rewind();                                   // find the last dewptNN.log file in /datalog/dlogNN folder  
  fileName = "";
  while (file.openNext(sd.vwd(), O_READ)) {
    if (file.isFile()) {
      file.getSFN(tempFileName);
      if (String(tempFileName).substring(1,5) == "dewpt") {
        fileName = String(tempFileName);
      }
    }
    file.close();
  }

  if (fileName == "") {
    fileName = "dewpt01.log";
  }

  fullPathLogFile = fullPathLogFile + "/" + fileName;

  return fullPathLogFile;
}

