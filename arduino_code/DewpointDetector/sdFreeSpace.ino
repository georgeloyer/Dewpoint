/* boolean sdFreeSpace()
 *  checks for free space by using cluster counts from SdFat library
 */

boolean sdFreeSpace() {
  uint32_t totalClusterCount = sd.vol()->clusterCount();                  // count of total clusters
  uint32_t freeClusterCount = sd.vol()->freeClusterCount();               // count of free clusters
  float fs = 0.000512*freeClusterCount*sd.vol()->blocksPerCluster();      // MB of free space 
  float ts = 0.000512*totalClusterCount*sd.vol()->blocksPerCluster();     // MB of total space on SD card
  if (freeClusterCount < 0.20*totalClusterCount) {                        // 80% rule
    if (DEBUG) {
      Serial.println("SD card is over 80% full.");
      Serial.print("Remaining Free Space is: ");
      Serial.print(fs);
      Serial.print("  Total space on SD card is: ");
      Serial.println(ts);
    }
    return false;
  } else {
    if (DEBUG) {
      Serial.println("SD card is less than 80% full.");
      Serial.print("Remaining Free Space is: ");
      Serial.print(fs);
      Serial.print("  Total space on SD card is: ");
      Serial.println(ts);
    }
    return true;
  }
}

