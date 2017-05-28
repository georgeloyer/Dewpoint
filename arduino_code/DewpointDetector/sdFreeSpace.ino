/* boolean sdFreeSpace()
 *  checks for free space by using cluster counts from SdFat library
 */

boolean sdFreeSpace() {
  uint32_t totalClusterCount = sd.vol()->clusterCount();                  // count of total clusters
  uint32_t freeClusterCount = sd.vol()->freeClusterCount();               // count of free clusters
  uint32_t blocksPerCluster = 64;
  float fs = freeClusterCount*blocksPerCluster/1024/1024;                  // MB of free space 
  float ts = totalClusterCount*blocksPerCluster/1024/1024;                 // MB of total space on SD card
  if (freeClusterCount < 0.20*totalClusterCount) {                        // 80% rule
    if (DEBUG) {
      Serial.println("SD card is over 80% full.");
      Serial.print("Remaining Free Space is: ");
      Serial.print(fs);
      Serial.print(" MB. Total space on SD card is: ");
      Serial.print(ts);
      Serial.println(" MB.");
    }
    return false;
  }
  if (DEBUG) {
    Serial.println("SD card is less than 80% full.");
    Serial.print("Remaining Free Space is: ");
    Serial.print(fs);
    Serial.print(" MB. Total space on SD card is: ");
    Serial.print(ts);
    Serial.println(" MB.");
    Serial.print(blocksPerCluster);
    Serial.println(" blocks per cluster");
  }
  return true;
}

