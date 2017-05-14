/* boolean startSdFat()
 *  from SdInfo.ino example 
 *  we assume there is no other SPI device (otherwise we should disable that device so we don't interfere
 *  and don't get interference 
 *  
 *  initializes the SD card and checks that the SD card is not more than 80% full
 */

boolean startSdFat() {
  if (DEBUG) {
    Serial.print("Starting SD card.");
  }

  #if USE_SDIO
    if (!sd.cardBegin()) {
      if (DEBUG) {
        Serial.println("cardBegin failed. SD card is not present.");
        Serial.println("Operation will continue but no SD logging will occur.");
      }
      return false;
    }
  #else  // USE_SDIO
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!sd.cardBegin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
      if (DEBUG) {
        Serial.println("cardBegin failed. SD card is not present.");
        Serial.println("Operation will continue but no SD logging will occur.");
      }
      return false;
  }
  #endif  // USE_SDIO 

  if (! sdFreeSpace) {
    return false;                                 // free space is less than 80% of SD disk
  }
  
  if (DEBUG) {
    Serial.println("SD card started successfully and there's room on the disk to write logs.");
  }
  return true;
}

