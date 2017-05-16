/*
 * rollLogFile(String fullPathLogFile)
 * 
 * this function is called when the program detects that the size of the current log
 * file exceeds the file size limit configured for log files.
 * 
 * The log rotation is intended to limit the number of log files per folder to 99 and
 * the number of folder to 999. Once that limit is reached, the rotation wraps around.
 * 
 * The first folder number is 001 and the first log file in a folder is 01
 */

String rollLogFile (String fullPathLogFile) {
  SdFile newFile;
  
  String logFileName = fullPathLogFile.substring(fullPathLogFile.lastIndexOf("/")+1);
  String folderName = fullPathLogFile.substring(fullPathLogFile.indexOf("/", 2)+1, fullPathLogFile.lastIndexOf("/"));
  String rootName = fullPathLogFile.substring(1, fullPathLogFile.indexOf("/", 2));
  int logFileCount = logFileName.substring(6,7).toInt();
  int folderCount = folderName.substring(5,7).toInt();

  if (logFileCount < 99) {                                              // increase log file number by one
    logFileCount++;
    if (logFileCount < 10) {
      logFileName = "dewpt0" + String(logFileCount) + ".log";
    } else {
      logFileName = "dewpt" + String(logFileCount) + ".log";
    }
  } else {                                                              // overflow log file number, move to next folder
    logFileName = "dewpt01.log";
    if (folderCount < 999) {
      folderCount++;
      if (folderCount < 10) {
        folderName = "dlog00" + String(folderCount);
      } else if (folderCount < 100) {
        folderName = "dlog0" + String(folderCount);
      } else {
        folderName = "dlog" + String(folderCount);
      } 
    } else {
      folderName = "dlog001";
    }
  }
  
  fullPathLogFile = rootName + "/" + folderName + "/" + logFileName;    // set the fullPathLogFile name by concatenating the components
  if (sd.exists(fullPathLogFile.c_str())) {                             // if the file exists, we are rolling over and we remove it
    sd.remove(fullPathLogFile.c_str());
  }
  newFile.open(fullPathLogFile.c_str(), O_CREAT | O_RDWR);                   // Open the file to create it, then close it
  newFile.close();

  if (DEBUG) {
    String errMsg = "INFO: Rollover to ";
    errMsg += fullPathLogFile;
    Serial.println(errMsg);
  }
  
  return fullPathLogFile;
}

