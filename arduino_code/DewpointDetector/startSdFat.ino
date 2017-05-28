/* boolean startSdFat()
 *  from SdInfo.ino example 
 *  we assume there is no other SPI device (otherwise we should disable that device so we don't interfere
 *  and don't get interference 
 *  
 *  initializes the SD card and checks that the SD card is not more than 80% full
 */

boolean startSdFat() {
  if (DEBUG) {
    Serial.println("Starting SD card.");
  }

  #if USE_SDIO
    if (!sd.begin()) {
      sd.initErrorHalt();
      if (DEBUG) {
        Serial.println("ERROR: begin() failed. SD card is not present.");
      }
      return false;
    }
  #else  // USE_SDIO
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
      sd.initErrorHalt();
      if (DEBUG) {
        Serial.println("ERROR: begin() failed. SD card is not present.");
      }
      return false;
  }
  #endif  // USE_SDIO 

  if (!sdFreeSpace()) {
    if (DEBUG) {
      Serial.println("ERROR: less than 80% free space on SD disk");
    }
    return false;                                 // free space is less than 80% of SD disk
  }
  
  if (DEBUG) {
    Serial.println("INFO: SD card started successfully and there's room on the disk to write logs.");
  }
  return true;
}

